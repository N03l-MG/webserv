/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:11 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/06 13:17:23 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"

class SocketManager
{
	private:
		
	public:
		std::vector<Socket*> sockets;
		SocketManager(std::vector<Server*> servers);
		~SocketManager();

		void run();
		void checkSockets(fd_set read_fds, std::map<int, Socket*> socket_map, std::map<int, int> client_to_server, int max_fd);
		std::vector<int> checkClient(fd_set read_fds, std::map<int, Socket*> socket_map, std::map<int, int> client_to_server);
};
