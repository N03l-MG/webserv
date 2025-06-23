/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:07 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/11 14:08:46 by nmonzon          ###   ########.fr       */
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
	for (Socket* socket : sockets)
		delete socket;
	sockets.clear();
}

void SocketManager::run() 
{
	fd_set read_fds;
	std::map<int, Socket*> socket_map;
	std::map<int, int> client_to_server; // Maps client FDs to server FDs
	int max_fd = -1;
	std::vector<int> to_remove;

	// Map socket file descriptors to Socket objects
	for (Socket* socket : sockets) {
		socket_map[socket->server_fd] = socket;
		if (socket->server_fd > max_fd)
			max_fd = socket->server_fd;
	}

	while (true) {
		FD_ZERO(&read_fds);

		// Add all server sockets to the read set
		for (const auto& pair : socket_map) {
			FD_SET(pair.first, &read_fds);
		}

		// Add all client sockets to the read set
		for (const auto& pair : client_to_server) {
			FD_SET(pair.first, &read_fds);
			if (pair.first > max_fd)
				max_fd = pair.first;
		}

		// Wait for activity on any socket
		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
			if (errno == EINTR)
				continue;
			std::cerr << "Select error: " << strerror(errno) << std::endl;
			break;
		}
		checkSockets(read_fds, socket_map, client_to_server, max_fd);

		// Remove closed client connections
		to_remove = checkClient(read_fds, socket_map, client_to_server);
		for (int fd : to_remove) {
			client_to_server.erase(fd);
		}
	}
}

void SocketManager::checkSockets(fd_set &read_fds, t_client_socket &sock_map, t_client_server &client_server_map, int &max_fd)
{
	// Check each socket for activity
	for (const auto &pair : sock_map) {
		int server_fd = pair.first;
		
		if (FD_ISSET(server_fd, &read_fds)) {
			// Handle new connection
			struct sockaddr_in client_addr;
			socklen_t client_len = sizeof(client_addr);
			
			int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
			if (client_fd < 0) {
				if (errno != EWOULDBLOCK)
					std::cerr << "Accept error: " << strerror(errno) << std::endl;
				continue;
			}

			// Set client socket to non-blocking
			int flags = fcntl(client_fd, F_GETFL, 0);
			fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

			// Add client socket to our maps
			client_server_map[client_fd] = server_fd;
			if (client_fd > max_fd)
				max_fd = client_fd;
		}
	}
}

std::vector<int> SocketManager::checkClient(fd_set &read_fds, t_client_socket &sock_map, t_client_server &client_server_map)
{
	// Check client sockets for activity
	std::vector<int> to_remove;
	for (const auto& pair : client_server_map) {
		int client_fd = pair.first;
		if (FD_ISSET(client_fd, &read_fds)) {
			char buffer[4096];
			ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
			if (bytes_read <= 0) {
				close(client_fd);
				to_remove.push_back(client_fd);
				continue;
			}
			buffer[bytes_read] = '\0';
			int server_fd = pair.second;
			Socket* socket = sock_map[server_fd];
			socket->server->respond(client_fd, std::string(buffer));
			close(client_fd);
			to_remove.push_back(client_fd);
		}
	}
	return to_remove;
}
