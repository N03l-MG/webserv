/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:11 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/25 08:43:16 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"

typedef std::map<int, Socket*> t_client_socket;
typedef std::map<int, int> t_client_server;

class SocketManager
{
	private:
		std::vector<Socket*> sockets;

		// void initializeSockets(std::map<int, Socket*> &socket_map, int &max_fd);
		// void prepareFdSets(fd_set &read_fds, const std::map<int, Socket*> &socket_map,
		// 				const std::map<int, int> &client_to_server, int &max_fd);
		// int handleNewConnection(int server_fd, std::map<int, int> &client_to_server, int &max_fd);
		// void handleClientData(int client_fd, Socket* socket, std::vector<int> &to_remove);
		std::vector<int> checkClient(fd_set &read_fds, t_client_socket &sock_map, t_client_server &client_server_map);
		void checkSockets(fd_set &read_fds, t_client_socket &sock_map, t_client_server &client_server_map, int &max_fd);

	public:
		SocketManager(std::vector<Server*> servers);
		~SocketManager();

		void run();
};
