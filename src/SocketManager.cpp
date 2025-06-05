/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:07 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/05 12:17:50 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"
#include <arpa/inet.h>
#include <cerrno>

SocketManager::SocketManager() : max_fd(0) {}

SocketManager::~SocketManager()
{
	for (int sock : server_sockets)
		close(sock);
	for (auto& pair : client_sockets)
		close(pair.first);
}

void SocketManager::setNonBlocking(int sockfd)
{
	int flags = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int SocketManager::createServerSocket(int port, const std::string& host)
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw std::runtime_error("Failed to create socket");

	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(host.c_str());

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed with error: " << strerror(errno) << std::endl;
        throw std::runtime_error("Bind failed");
    }

	if (listen(server_fd, SOMAXCONN) < 0)
		throw std::runtime_error("Listen failed");

	setNonBlocking(server_fd);
	return server_fd;
}

void SocketManager::initializeSockets(const std::vector<Server*>& servers)
{
	for (Server *server : servers)
	{
		int sock = createServerSocket(server->getPort(), server->getHost());
		server_sockets.push_back(sock);
		max_fd = std::max(max_fd, sock);
	}
}

void SocketManager::handleNewConnection(int server_socket)
{
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	
	int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
	if (client_socket < 0)
		return;

	setNonBlocking(client_socket);
	client_sockets[client_socket] = server_socket;
	max_fd = std::max(max_fd, client_socket);
}

void SocketManager::handleClientData(int client_socket)
{
	char buffer[1024] = {0};
	ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);

	if (bytes_read <= 0) {
		close(client_socket);
		client_sockets.erase(client_socket);
		return;
	}

	send(client_socket, buffer, bytes_read, 0);
}

void SocketManager::run()
{
	while (true)
	{
		// Add timeout to select to allow for checking shutdown condition
		struct timeval timeout;
		timeout.tv_sec = 1;  // 1 second timeout
		timeout.tv_usec = 0;

		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);

		// Add server sockets to fd sets
		for (int sock : server_sockets)
			FD_SET(sock, &read_fds);

		// Add client sockets to fd sets
		for (const auto& pair : client_sockets)
		{
			FD_SET(pair.first, &read_fds);
			FD_SET(pair.first, &write_fds);
		}

		 // Modified select call with timeout
		int activity = select(max_fd + 1, &read_fds, &write_fds, NULL, &timeout);
		
		if (activity < 0) {
			if (errno == EINTR)  // Interrupted system call
				continue;
			break;  // Error occurred
		}

		// Check server sockets for new connections
		for (int server_sock : server_sockets)
			if (FD_ISSET(server_sock, &read_fds))
				handleNewConnection(server_sock);

		// Check client sockets for data
		for (auto it = client_sockets.begin(); it != client_sockets.end();)
		{
			int client_sock = it->first;
			if (FD_ISSET(client_sock, &read_fds))
				handleClientData(client_sock);
			++it;
		}
	}

	// Cleanup on exit
	for (int sock : server_sockets)
		close(sock);
	
	for (const auto& pair : client_sockets)
		close(pair.first);
		
	server_sockets.clear();
	client_sockets.clear();
}
