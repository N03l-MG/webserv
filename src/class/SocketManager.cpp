/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:07 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/27 09:28:50 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

SocketManager::SocketManager(std::vector<Server*> servers)
{
	for (Server *server : servers)
		sockets.push_back(new Socket(server));
}

SocketManager::~SocketManager()
{
	for (Socket *socket : sockets)
		delete socket;
	sockets.clear();
}

void SocketManager::initializeServerSockets()
{
	min_timeout = std::numeric_limits<size_t>::max();
	
	for (Socket *socket : sockets) {
		int fd = socket->server_fd;
		fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

		pollfd pfd = {fd, POLLIN, 0};
		poll_fds.push_back(pfd);
		fd_to_socket[fd] = socket;
		min_timeout = std::min(min_timeout, socket->server->getTimeout());
	}
}

void SocketManager::handleNewConnection(pollfd &pfd, time_t now)
{
	sockaddr_in client_addr{};
	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(pfd.fd, (sockaddr*)&client_addr, &client_len);
	
	if (client_fd < 0) {
		std::cerr << "Accept failed: " << strerror(errno) << "\n";
		return;
	}
	
	fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK);
	poll_fds.push_back({client_fd, POLLIN, 0});
	client_to_server[client_fd] = fd_to_socket[pfd.fd]->server;
	client_last_active[client_fd] = now;
	client_buffers[client_fd] = "";
}

bool SocketManager::processRequest(int fd, std::string &request)
{
	size_t headers_end = request.find("\r\n\r\n");
	if (headers_end != std::string::npos) {
		size_t body_start = headers_end + 4;
		size_t content_length = 0;

		size_t cl_pos = request.find("Content-Length: ");
		if (cl_pos != std::string::npos) {
			size_t cl_end = request.find("\r\n", cl_pos);
			std::string cl_str = request.substr(cl_pos + 16, cl_end - (cl_pos + 16));
			content_length = std::stoul(cl_str);
		}

		if (content_length == 0 || request.size() >= body_start + content_length) {
			Server *server = client_to_server[fd];
			server->respond(fd, request);
			return true;
		}
	}
	return false;
}

void SocketManager::cleanupClient(int fd, size_t &i)
{
	close(fd);
	client_to_server.erase(fd);
	client_last_active.erase(fd);
	client_buffers.erase(fd);
	poll_fds.erase(poll_fds.begin() + i--);
}

void SocketManager::handleClientData(size_t &i, pollfd &pfd, time_t now)
{
	std::string &request = client_buffers[pfd.fd];
	const size_t chunk_size = 8192;
	char chunk[chunk_size];
	ssize_t bytes_read;
	bool headers_complete = false;
	size_t content_length = 0;

	while ((bytes_read = recv(pfd.fd, chunk, chunk_size - 1, 0)) > 0) {
		chunk[bytes_read] = '\0';
		request.append(chunk, bytes_read);

		if (!headers_complete) {
			size_t header_end = request.find("\r\n\r\n");
			if (header_end != std::string::npos) {
				headers_complete = true;
				size_t cl_pos = request.find("Content-Length: ");
				if (cl_pos != std::string::npos) {
					size_t cl_end = request.find("\r\n", cl_pos);
					content_length = std::stoul(request.substr(cl_pos + 16, cl_end - (cl_pos + 16)));
				}
			}
		}

		if (headers_complete) {
			size_t headers_size = request.find("\r\n\r\n") + 4;
			if (content_length == 0 || request.size() >= headers_size + content_length)
				break;
		}
	}
	std::ostringstream oss;
	oss << "Request of size " << request.size() << " bytes received from client (fd) " << pfd.fd << " in server " << client_to_server[pfd.fd];
	log(LOG_LOG, oss.str());

	if (bytes_read == 0 || (bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK)) {
		cleanupClient(pfd.fd, i);
		return;
	}
	if (processRequest(pfd.fd, request)) {
		cleanupClient(pfd.fd, i);
		return;
	}
	client_last_active[pfd.fd] = now;
}

void SocketManager::handleErrors(size_t &i, pollfd &pfd)
{
	if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL))
		cleanupClient(pfd.fd, i);
}

void SocketManager::checkTimeouts(time_t now)
{
	for (size_t i = 0; i < poll_fds.size(); ++i) {
		int fd = poll_fds[i].fd;
		if (client_to_server.count(fd)) {
			Server *server = client_to_server[fd];
			time_t last = client_last_active[fd];
			if (difftime(now, last) > server->getTimeout()) {
				std::cout << "Timeout: closing client fd " << fd << "\n";
				cleanupClient(fd, i);
			}
		}
	}
}

void SocketManager::run()
{
	initializeServerSockets();

	while (true) {
		int ret = poll(poll_fds.data(), poll_fds.size(), 100);
		if (ret < 0) {
			if (errno == EINTR) continue;
			std::cerr << "poll error: " << strerror(errno) << "\n";
			break;
		}

		time_t now = time(nullptr);
		for (size_t i = 0; i < poll_fds.size(); ++i) {
			pollfd &pfd = poll_fds[i];
			if ((pfd.revents & POLLIN) && fd_to_socket.count(pfd.fd)) {
				handleNewConnection(pfd, now);
				continue;
			}
			if ((pfd.revents & POLLIN) && client_to_server.count(pfd.fd)) {
				handleClientData(i, pfd, now);
				continue;
			}
			handleErrors(i, pfd);
		}
		checkTimeouts(now);
	}
}
