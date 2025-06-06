/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:32:39 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 10:16:32 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include "webserv.hpp"
#include "Server.hpp"

//	Class
class	WebServer
{
	private:
		bool					is_running;
		std::string				config_path;
		std::vector<t_tokens>	tokens;
		std::vector<Server*>	servers;

		void	parseConfig();

	public:
		WebServer();
		~WebServer();

		void	setTokens(std::vector<t_tokens>);
		int		addServer(Server *new_server);
		t_tokens	*getToken(size_t index);
		std::vector<t_tokens>	getTokens();
		Server	*getServer(size_t index);
		std::vector<Server*>	getServer();

		void	start();
		void	shutdown();
};