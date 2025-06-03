/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:34:44 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/03 12:29:53 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

//	Constructor
WebServer::WebServer()
{
	is_running = false;
}


//	Destructor
WebServer::~WebServer()
{
	//delete all servers to avoid leaking
	for (Server* serv : servers)
		delete serv;
	servers.clear();
}

//	Setters


//	Getters
Server	*WebServer::getServer(size_t index)
{
	if (index < servers.size())
		return (servers[index]);
	return (NULL);
}


std::vector<Server*>	WebServer::getServer()
{
	return (servers);
}


//	Add new server
int	WebServer::addServer(Server *new_server)
{
	if (!new_server)
		return (1);
	servers.push_back(new_server);
	return (0);
}


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
			Server *new_server = new Server;
			if (addServer(new_server) == 0)
				new_server->configure(tokens, i);
		}
	}
}
