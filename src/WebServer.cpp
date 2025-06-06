/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:34:44 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 10:18:46 by jgraf            ###   ########.fr       */
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
	{
		std::cout << "Delete server" << std::endl;
		delete serv;
	}
	servers.clear();
}

//	Setters
void	WebServer::setTokens(std::vector<t_tokens> tokens)
{
	this->tokens = tokens;
}

int	WebServer::addServer(Server *new_server)
{
	if (!new_server)
		return (1);
	servers.push_back(new_server);
	return (0);
}


//	Getters
t_tokens	*WebServer::getToken(size_t index)
{
	if (index < tokens.size())
		return (&tokens[index]);
	return (NULL);
}

std::vector<t_tokens>	WebServer::getTokens()
{
	return (this->tokens);
}

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


//	Start
void	WebServer::start()
{
	is_running = true;
	parseConfig();
}


//	Parse Input
void	WebServer::parseConfig()
{
	for (size_t i = 0; i < tokens.size(); i++)
	{
		std::cout << tokens[i].type << "\t" << tokens[i].token << std::endl;
		/*if (tokens[i] == "server")
		{
			Server *new_server = new Server;
			if (addServer(new_server) == 0)
				new_server->configure(tokens, i);
		}*/
	}
}