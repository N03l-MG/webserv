/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:11 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/26 14:43:11 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"
#include "include.hpp"

typedef std::map<int, Socket*> t_client_socket;
typedef std::map<int, int> t_client_server;

class SocketManager
{
	private:
		std::vector<Socket*> sockets;
		std::vector<int> checkClient(fd_set &read_fds, t_client_socket &sock_map, t_client_server &client_server_map);
		std::map<int, time_t> client_last_activity;;
		void checkSockets(fd_set &read_fds, t_client_socket &sock_map, t_client_server &client_server_map, int &max_fd);

	public:
		SocketManager(std::vector<Server*> servers);
		~SocketManager();

		void run();
};
