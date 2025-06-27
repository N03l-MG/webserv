/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:07 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/27 08:49:32 by jgraf            ###   ########.fr       */
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
	struct timeval	timeout;

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

		// Setup timeval
		timeout.tv_sec = TIMEOUT;
		timeout.tv_usec = 0;

		// Wait for activity on any socket
		if (select(max_fd + 1, &read_fds, NULL, NULL, &timeout) < 0) {
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

// Modify checkSockets to disconnect idle clients
void SocketManager::checkSockets(fd_set &read_fds, t_client_socket &sock_map, t_client_server &client_server_map, int &max_fd)
{
    // Get the current time
    time_t current_time = time(NULL);

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
            client_last_activity[client_fd] = current_time; // Track activity time
            if (client_fd > max_fd)
                max_fd = client_fd;
        }
    }

    // Disconnect idle clients
    std::vector<int> idle_clients;
    for (const auto &pair : client_last_activity) {
        int client_fd = pair.first;
        time_t last_activity = pair.second;

        // Check if the client has been idle for longer than TIMEOUT seconds
        if (current_time - last_activity > TIMEOUT) {
            idle_clients.push_back(client_fd);
        }
    }

    // Close idle client connections
    for (int client_fd : idle_clients) {
        close(client_fd);
        client_server_map.erase(client_fd);
        client_last_activity.erase(client_fd);
        std::cout << "Disconnected idle client: " << client_fd << std::endl;
    }
}

std::vector<int> SocketManager::checkClient(fd_set &read_fds, t_client_socket &sock_map, t_client_server &client_server_map)
{
	std::vector<int> to_remove;
	for (const auto& pair : client_server_map) {
		int client_fd = pair.first;
		if (FD_ISSET(client_fd, &read_fds)) {
			std::string request;
			const size_t chunk_size = 8192;
			char chunk[chunk_size];
			size_t total_bytes = 0;
			bool headers_complete = false;
			size_t content_length = 0;

			while (true) {
				ssize_t bytes_read = recv(client_fd, chunk, chunk_size - 1, 0);
				if (bytes_read <= 0) {
					if (bytes_read == EAGAIN || errno == EWOULDBLOCK) {
						continue;
					}
					close(client_fd);
					to_remove.push_back(client_fd);
					break;
				} else if (bytes_read == 0) {
					close(client_fd);
					to_remove.push_back(client_fd);
					break;
				}

				chunk[bytes_read] = '\0';
				request.append(chunk, bytes_read);
				total_bytes += bytes_read;
				if (!headers_complete) {
					size_t header_end = request.find("\r\n\r\n");
					if (header_end != std::string::npos) {
						headers_complete = true;
						
						// Find Content-Length
						size_t cl_pos = request.find("Content-Length: ");
						if (cl_pos != std::string::npos) {
							size_t cl_end = request.find("\r\n", cl_pos);
							std::string cl_str = request.substr(cl_pos + 16, cl_end - (cl_pos + 16));
							content_length = std::stoul(cl_str);
						}
					}
				}
				if (headers_complete) {
					size_t headers_size = request.find("\r\n\r\n") + 4;
					if (content_length == 0 || total_bytes >= headers_size + content_length) {
						break;
					}
				}
			}

			if (!request.empty()) {
				int server_fd = pair.second;
				Socket* socket = sock_map[server_fd];
				std::cout << "Received request of size: " << request.size() << " bytes, from client: " << client_fd << std::endl;
				//std::cout << "Full Request:\n" << request << std::endl;
				if (headers_complete && total_bytes >= content_length)
					socket->server->respond(client_fd, request);
				close(client_fd);
				to_remove.push_back(client_fd);
			}
		}
	}
	return to_remove;
}
