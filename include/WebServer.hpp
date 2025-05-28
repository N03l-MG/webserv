/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:32:39 by jgraf             #+#    #+#             */
/*   Updated: 2025/05/28 10:46:35 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include "webserv.hpp"
//#include "ServerConfig.hpp"
//#include "ServerSocket.hpp"
//#include "ClientConnection.hpp"
//#include "CgiProcess.hpp"
//#include "Poller.hpp"

class	WebServer
{
	private:
		std::string							config_path;
		bool								is_running;
		//std::vector<ServerConfig>			servers;
		//std::map<int, ServerSocket>			listeningSockets;
		//std::map<int, ClientConnection*>	clients;
		//std::map<pid_t, CgiProcess>			active_cgi_Processes;
		//Poller								poller;

		void	parseConfig();
		void	setupSockets();
		void	runEventLoop();
		void	acceptNewClient(int listenFd);
		void	handleClientRead(int clientFd);
		void	handleClientWrite(int clientFd);
		void	cleanupClient(int clientFd);
		void	handleCgiOutput(pid_t pid);

	public:
		WebServer();
		~WebServer();

		void	start(std::vector<std::string> &tokens);
		void	shutdown();
};