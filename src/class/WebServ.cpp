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
	{
		std::cout << "Delete server:\t" << serv << std::endl;
		delete serv;
	}
	servers.clear();
}

//	Setters
void	WebServ::setTokens(t_vectok tokens) { this->tokens = tokens;}
int		WebServ::addServer(Server *new_server)
{
	if (!new_server)
		return (1);
	servers.push_back(new_server);
	return (0);
}


//	Getters
bool		WebServ::isRunning() { return (this->is_running); }
t_vectok	WebServ::getTokens() { return (this->tokens);}
t_tokens	*WebServ::getToken(size_t index)
{
	if (index < tokens.size())
		return (&tokens[index]);
	return (NULL);
}
std::vector<Server*>	WebServ::getServer() {return (servers);}
Server	*WebServ::getServer(size_t index)
{
	if (index < servers.size())
		return (servers[index]);
	return (NULL);
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


//	Start
void	WebServ::start()
{
	parseConfig();

	socketManager = new SocketManager(servers);
	socketManager->run();
}


//	Shutdown
void WebServ::shutdown()
{
	delete socketManager;
}