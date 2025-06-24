/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:11 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/24 16:03:00 by nmonzon          ###   ########.fr       */
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

		std::vector<int> checkClient(fd_set &read_fds, t_client_socket &sock_map, t_client_server &client_server_map);
		void checkSockets(fd_set &read_fds, t_client_socket &sock_map, t_client_server &client_server_map, int &max_fd);

	public:
		SocketManager(std::vector<Server*> servers);
		~SocketManager();

		void run();
};
