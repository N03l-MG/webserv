/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:34:44 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 11:33:35 by nmonzon          ###   ########.fr       */
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
		delete serv;
	servers.clear();
}

//	Setters


//	Getters
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


//	Add new server
int	WebServ::addServer(Server *new_server)
{
	if (!new_server)
		return (1);
	servers.push_back(new_server);
	return (0);
}


//	Start
void	WebServ::start(t_vecstr &tokens)
{
	this->tokens = tokens;
	parseConfig();

	socketManager = new SocketManager(servers);
	socketManager->run();
}


//	Parse Input
void	WebServ::parseConfig()
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

//	Shutdown
void WebServ::shutdown()
{
	delete socketManager;
}
