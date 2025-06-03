/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:32:39 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/03 12:29:25 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include "webserv.hpp"
#include "Server.hpp"


class	WebServer
{
	private:
		std::string				config_path;
		t_vecstr				tokens;
		bool					is_running;
		std::vector<Server*>	servers;

		void	parseConfig();

	public:
		WebServer();
		~WebServer();

		int		addServer(Server *new_server);
		Server	*getServer(size_t index);
		std::vector<Server*>	getServer();

		void	start(t_vecstr &tokens);
		void	shutdown();
};
