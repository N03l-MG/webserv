/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:11 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/03 12:55:11 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <map>
#include "Server.hpp"

class SocketManager
{
	private:
		std::vector<int> server_sockets;
		std::map<int, int> client_sockets; // socket fd -> associated server socket
		fd_set read_fds;
		fd_set write_fds;
		int max_fd;

		void setNonBlocking(int sockfd);
		void handleNewConnection(int server_socket);
		void handleClientData(int client_socket);

	public:
		SocketManager();
		~SocketManager();

		int createServerSocket(int port, const std::string& host);
		void initializeSockets(const std::vector<Server*>& servers);
		void run();
};
