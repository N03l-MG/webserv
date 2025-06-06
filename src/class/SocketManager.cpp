/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:07 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/06 16:34:59 by nmonzon          ###   ########.fr       */
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

void SocketManager::initializeSockets(std::map<int, Socket*> &socket_map, int &max_fd) 
{
	for (Socket* socket : sockets) {
		int server_fd = socket->server_fd;
		socket_map[server_fd] = socket;
		if (server_fd > max_fd) {
			max_fd = server_fd;
		}
	}
}

void SocketManager::prepareFdSets(fd_set &read_fds, const std::map<int, Socket*> &socket_map,
								const std::map<int, int> &client_to_server, int &max_fd) 
{
	FD_ZERO(&read_fds);

	// Add server sockets
	for (const auto &pair : socket_map) {
		FD_SET(pair.first, &read_fds);
	}

	// Add client sockets
	for (const auto &pair : client_to_server) {
		FD_SET(pair.first, &read_fds);
		if (pair.first > max_fd) {
			max_fd = pair.first;
		}
	}
}

int SocketManager::handleNewConnection(int server_fd, std::map<int, int> &client_to_server, int &max_fd) 
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	
	int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0) {
		if (errno != EWOULDBLOCK) {
			std::cerr << "Accept error: " << strerror(errno) << std::endl;
		}
		return -1;
	}

	// Set non-blocking
	int flags = fcntl(client_fd, F_GETFL, 0);
	fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

	// Add to tracking
	client_to_server[client_fd] = server_fd;
	if (client_fd > max_fd) {
		max_fd = client_fd;
	}
	return client_fd;
}

void SocketManager::handleClientData(int client_fd, Socket* socket, std::vector<int> &to_remove) 
{
	try {
		socket->server->respond(client_fd);
	} catch (const std::exception &e) {
		std::cerr << "Error handling client: " << e.what() << std::endl;
		close(client_fd);
		to_remove.push_back(client_fd);
	}
}

void SocketManager::run() 
{
	fd_set read_fds;
	std::map<int, Socket*> socket_map;
	std::map<int, int> client_to_server;
	int max_fd = -1;

	initializeSockets(socket_map, max_fd);

	while (true) {
		prepareFdSets(read_fds, socket_map, client_to_server, max_fd);

		int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
		if (activity < 0) {
			if (errno == EINTR) continue;
			std::cerr << "Select error: " << strerror(errno) << std::endl;
			break;
		}

		// Handle new connections
		for (const auto &pair : socket_map) {
			if (FD_ISSET(pair.first, &read_fds)) {
				handleNewConnection(pair.first, client_to_server, max_fd);
			}
		}

		// Handle existing connections
		std::vector<int> to_remove;
		for (const auto &pair : client_to_server) {
			if (FD_ISSET(pair.first, &read_fds)) {
				handleClientData(pair.first, socket_map[pair.second], to_remove);
			}
		}

		// Clean up closed connections
		for (int fd : to_remove) {
			client_to_server.erase(fd);
		}
	}
}
