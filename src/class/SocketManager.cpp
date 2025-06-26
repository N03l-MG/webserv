/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:07 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/26 18:18:28 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

SocketManager::SocketManager(std::vector<Server*> servers) {
	for (Server* server : servers)
		sockets.push_back(new Socket(server));
}

SocketManager::~SocketManager() {
	for (Socket* socket : sockets)
		delete socket;
	sockets.clear();
}

void SocketManager::run() {
	std::vector<pollfd> poll_fds;
	std::map<int, Socket*> fd_to_socket;
	std::map<int, Server*> client_to_server;
	std::map<int, time_t> client_last_active;
	std::map<int, std::string> client_buffers;

	size_t min_timeout = std::numeric_limits<size_t>::max();

	// Initialize poll_fds with server sockets
	for (Socket* socket : sockets) {
		int fd = socket->server_fd;
		fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

		pollfd pfd = {fd, POLLIN, 0};
		poll_fds.push_back(pfd);
		fd_to_socket[fd] = socket;
		min_timeout = std::min(min_timeout, socket->server->getTimeout());
	}

	while (true) {
		int timeout_ms = static_cast<int>(min_timeout * 1000);
		int ret = poll(poll_fds.data(), poll_fds.size(), timeout_ms);
		if (ret < 0) {
			if (errno == EINTR) continue;
			std::cerr << "poll error: " << strerror(errno) << "\n";
			break;
		}

		time_t now = time(nullptr);

		for (size_t i = 0; i < poll_fds.size(); ++i) {
			pollfd& pfd = poll_fds[i];

			// New connection on server socket
			if ((pfd.revents & POLLIN) && fd_to_socket.count(pfd.fd)) {
				sockaddr_in client_addr{};
				socklen_t client_len = sizeof(client_addr);
				int client_fd = accept(pfd.fd, (sockaddr*)&client_addr, &client_len);
				if (client_fd < 0) {
					std::cerr << "Accept failed: " << strerror(errno) << "\n";
					continue;
				}
				fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK);

				poll_fds.push_back({client_fd, POLLIN, 0});
				client_to_server[client_fd] = fd_to_socket[pfd.fd]->server;
				client_last_active[client_fd] = now;
				client_buffers[client_fd] = "";
				continue;
			}

			// Data from client
			if ((pfd.revents & POLLIN) && client_to_server.count(pfd.fd)) {
				std::string& request = client_buffers[pfd.fd];
				const size_t chunk_size = 8192;
				char chunk[chunk_size];
				ssize_t bytes_read;
				bool headers_complete = false;
				size_t content_length = 0;

				while ((bytes_read = recv(pfd.fd, chunk, chunk_size - 1, 0)) > 0) {
					chunk[bytes_read] = '\0';
					request.append(chunk, bytes_read);

					// Check headers
					size_t header_end = request.find("\r\n\r\n");
					if (!headers_complete && header_end != std::string::npos) {
						headers_complete = true;

						// Look for Content-Length
						size_t cl_pos = request.find("Content-Length: ");
						if (cl_pos != std::string::npos) {
							size_t cl_end = request.find("\r\n", cl_pos);
							std::string cl_str = request.substr(cl_pos + 16, cl_end - (cl_pos + 16));
							content_length = std::stoul(cl_str);
						}
					}

					if (headers_complete) {
						size_t headers_size = request.find("\r\n\r\n") + 4;
						if (content_length == 0 || request.size() >= headers_size + content_length) {
							break; // Full request read
						}
					}
				}

				if (bytes_read == 0 || (bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK)) {
					close(pfd.fd);
					client_to_server.erase(pfd.fd);
					client_last_active.erase(pfd.fd);
					client_buffers.erase(pfd.fd);
					poll_fds.erase(poll_fds.begin() + i--);
					continue;
				}

				// Check if request is complete
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
						// Full request received
						Server* server = client_to_server[pfd.fd];
						server->respond(pfd.fd, request);
						close(pfd.fd);
						client_to_server.erase(pfd.fd);
						client_last_active.erase(pfd.fd);
						client_buffers.erase(pfd.fd);
						poll_fds.erase(poll_fds.begin() + i--);
						continue;
					}
				}

				client_last_active[pfd.fd] = now;
			}

			// Error or hang-up
			if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
				close(pfd.fd);
				client_to_server.erase(pfd.fd);
				client_last_active.erase(pfd.fd);
				client_buffers.erase(pfd.fd);
				poll_fds.erase(poll_fds.begin() + i--);
				continue;
			}
		}

		// Timeout check
		for (size_t i = 0; i < poll_fds.size(); ++i) {
			int fd = poll_fds[i].fd;
			if (client_to_server.count(fd)) {
				Server* server = client_to_server[fd];
				time_t last = client_last_active[fd];
				if (difftime(now, last) > server->getTimeout()) {
					std::cout << "Timeout: closing client fd " << fd << "\n";
					close(fd);
					client_to_server.erase(fd);
					client_last_active.erase(fd);
					client_buffers.erase(fd);
					poll_fds.erase(poll_fds.begin() + i--);
				}
			}
		}
	}
}
