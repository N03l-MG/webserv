/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:32:39 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/02 11:38:13 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include "webserv.hpp"
#include "ServerConfig.hpp"
//#include "ServerSocket.hpp"
//#include "ClientConnection.hpp"
//#include "CgiProcess.hpp"
//#include "Poller.hpp"

class	WebServer
{
	private:
		std::string							config_path;
		t_vecstr							tokens;
		bool								is_running;
		std::vector<ServerConfig*>			servers;
		//std::map<int, ServerSocket>			listeningSockets;
		//std::map<int, ClientConnection*>	clients;
		//std::map<pid_t, CgiProcess>			active_cgi_Processes;
		//Poller								poller;

		void	parseConfig();
		int		addServer(ServerConfig *new_server);

	public:
		WebServer();
		~WebServer();

		void	start(t_vecstr &tokens);
		void	shutdown();
};