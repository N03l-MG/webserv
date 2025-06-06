/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:32:39 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 12:01:39 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include "webserv.hpp"
#include "Server.hpp"
#include "SocketManager.hpp"


class	WebServ
{
	private:
		std::string				config_path;
		t_vecstr				tokens;
		bool					is_running;
		std::vector<Server*>	servers;
		SocketManager			*socketManager;

		void	parseConfig();

	public:
		WebServ();
		~WebServ();

		int		addServer(Server *new_server);
		Server	*getServer(size_t index);
		std::vector<Server*>	getServer();

		void	start(t_vecstr &tokens);
		void	shutdown();
		bool	isRunning() const { return is_running; }
};
