/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:32:39 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 12:26:02 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include "include.hpp"
#include "Server.hpp"

//	Class
class	WebServ
{
	private:
		bool					is_running;
		std::string				config_path;
		t_vectok	tokens;
		std::vector<Server*>	servers;

		void	parseConfig();

	public:
		WebServ();
		~WebServ();

		void	setTokens(t_vectok);
		int		addServer(Server *new_server);
		t_tokens	*getToken(size_t index);
		t_vectok	getTokens();
		Server	*getServer(size_t index);
		std::vector<Server*>	getServer();

		void	start();
		void	shutdown();
};