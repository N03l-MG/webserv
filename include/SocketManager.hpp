/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:55:11 by nmonzon           #+#    #+#             */
/*   Updated: 2025/07/04 13:10:00 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "Socket.hpp"
#include "include.hpp"
#include "WebServ.hpp"
#include "Server.hpp"
#include <unordered_map>

class	Socket;

class	SocketManager
{
	private:
		std::vector<Socket*>		sockets;
		std::vector<pollfd>			poll_fds;
		std::map<int, Socket*>		fd_to_socket;
		std::map<int, Server*>		client_to_server;
		std::map<int, time_t>		client_last_active;
		std::map<int, std::string>	client_buffers;
		//std::unordered_map<int, std::string> client_write_buffers;
		size_t						min_timeout;

		void	initializeServerSockets();
		void	handleNewConnection(pollfd &pfd, time_t now);
		void	handleClientData(size_t &i, pollfd &pfd, time_t now);
		void	handleErrors(size_t &i, pollfd &pfd);
		void	checkTimeouts(time_t now);
		void	cleanupClient(int fd, size_t &i);
		bool	processRequest(int fd, std::string &request);

	public:
		SocketManager(std::vector<Server*> servers);
		~SocketManager();

		void	run();
};
