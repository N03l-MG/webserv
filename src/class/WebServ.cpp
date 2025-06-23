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
	log(LOG_LOG, "WebServ created!");
	is_running = false;
}

//	Destructor
WebServ::~WebServ()
{
	//delete all servers to avoid leaking
	for (Server *serv : servers)
		delete serv;
	servers.clear();
	log(LOG_LOG, "WebServ destroyed!");
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
<<<<<<< HEAD:src/class/WebServ.cpp
Server	*WebServ::getServer(size_t index)
=======
bool		WebServ::isRunning() { return (this->is_running); }
t_vectok	WebServ::getTokens() { return (this->tokens);}
t_tokens	*WebServ::getToken(size_t index)
>>>>>>> master:src/WebServer.cpp
{
	if (index < tokens.size())
		return (&tokens[index]);
	return (NULL);
}
<<<<<<< HEAD:src/class/WebServ.cpp


std::vector<Server*>	WebServ::getServer()
=======
std::vector<Server*>	WebServ::getServer() {return (servers);}
Server	*WebServ::getServer(size_t index)
>>>>>>> master:src/WebServer.cpp
{
	if (index < servers.size())
		return (servers[index]);
	return (NULL);
}


<<<<<<< HEAD:src/class/WebServ.cpp
//	Add new server
int	WebServ::addServer(Server *new_server)
=======
//	Parse Input
void	WebServ::parseConfig()
>>>>>>> master:src/WebServer.cpp
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
<<<<<<< HEAD:src/class/WebServ.cpp
void	WebServ::start(t_vecstr &tokens)
{
	this->tokens = tokens;
=======
void	WebServ::start()
{
>>>>>>> master:src/WebServer.cpp
	parseConfig();

	socketManager = new SocketManager(servers);
	socketManager->run();
}


<<<<<<< HEAD:src/class/WebServ.cpp
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
=======
//	Shutdown
void WebServ::shutdown()
{
	delete socketManager;
}
>>>>>>> master:src/WebServer.cpp
