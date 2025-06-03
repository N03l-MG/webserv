/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:34:44 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/03 13:04:17 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include "SocketManager.hpp"

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
	this->tokens = tokens;
	parseConfig();

	try {
		SocketManager socketManager;
		socketManager.initializeSockets(servers);
		socketManager.run();
	} catch (const std::exception& e) {
		std::cerr << "Socket error: " << e.what() << std::endl;
		shutdown();
	}
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


//	Shutdown
void WebServer::shutdown()
{
	if (!is_running)
		return;
		
	is_running = false;
	
	// Clean up all servers
	for (Server* server : servers)
		if (server)
			delete server;
	servers.clear();
	
	std::cout << "Server shutdown complete" << std::endl;
}
