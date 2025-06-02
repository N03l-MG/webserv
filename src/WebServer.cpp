/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:34:44 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/02 11:55:24 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

//	Constructor
WebServer::WebServer()
{
	is_running = false;
}


//	Destructor
WebServer::~WebServer() {}


//	Start
void	WebServer::start(t_vecstr &tokens)
{
	is_running = true;
	this->tokens = tokens;
	parseConfig();
}


//	Parse Input
void	WebServer::parseConfig()
{
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "server")
		{
			ServerConfig *new_server = new ServerConfig;
			if (addServer(new_server) == 0)
				new_server->configure(tokens, i);
			std::cout << new_server->getListen() << "\n" << new_server->getHost() << "\n" << new_server->getRoot() << "\n" << new_server->getIndex() << std::endl;
		}
	}
}


//	Add new server
int	WebServer::addServer(ServerConfig *new_server)
{
	if (!new_server)
		return (1);
	servers.push_back(new_server);
	return (0);
}