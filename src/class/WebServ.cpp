/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:34:44 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 12:54:26 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

//	Constructor
WebServ::WebServ()
{
	is_running = false;
}


//	Destructor
WebServ::~WebServ()
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
void	WebServ::setTokens(t_vectok tokens)
{
	this->tokens = tokens;
}

int	WebServ::addServer(Server *new_server)
{
	if (!new_server)
		return (1);
	servers.push_back(new_server);
	return (0);
}


//	Getters
t_tokens	*WebServ::getToken(size_t index)
{
	if (index < tokens.size())
		return (&tokens[index]);
	return (NULL);
}

t_vectok	WebServ::getTokens()
{
	return (this->tokens);
}

Server	*WebServ::getServer(size_t index)
{
	if (index < servers.size())
		return (servers[index]);
	return (NULL);
}

std::vector<Server*>	WebServ::getServer()
{
	return (servers);
}


//	Start
void	WebServ::start()
{
	is_running = true;
	parseConfig();
}


//	Parse Input
void	WebServ::parseConfig()
{
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].type == TOK_DIRECTIVE && tokens[i].token == "server")
		{
			Server *new_server = new Server;
			new_server->configure(tokens, ++i);
			servers.push_back(new_server);
		}
	}
}