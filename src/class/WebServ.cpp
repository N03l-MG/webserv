#include "WebServ.hpp"

//	Constructor
WebServ::WebServ()
{
	log(LOG_INFO, "WebServ created!");
	is_running = false;
}


//	Destructor
WebServ::~WebServ()
{
	//delete all servers to avoid leaking
	for (Server *serv : servers)
		delete serv;
	servers.clear();
	log(LOG_INFO, "WebServ destroyed!");
}

//	Setters
void	WebServ::setTokens(t_vectok tokens)	{ this->tokens = tokens; }
int		WebServ::addServer(Server *new_server)
{
	if (!new_server)
		return (1);
	servers.push_back(new_server);
	return (0);
}


//	Getters
bool		WebServ::isRunning()				{ return (this->is_running); }
t_vectok	WebServ::getTokens()				{ return (this->tokens); }
t_tokens	*WebServ::getToken(size_t index)
{
	if (index < tokens.size())
		return (&tokens[index]);
	return (NULL);
}
std::vector<Server*>	WebServ::getServer()	{ return (servers); }
Server	*WebServ::getServer(size_t index)
{
	if (index < servers.size())
		return (servers[index]);
	return (NULL);
}


//	Remove a server from the list
void	WebServ::rmServer(Server *server)
{
	for (std::vector<Server*>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
		if (*it == server)
		{
			this->servers.erase(it);
			delete server;
			return;
		}
}


//	Parse Input
void	WebServ::parseConfig()
{
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].type == TOK_DIRECTIVE && tokens[i].token == "server")
		{
			Server	*new_server = new Server;
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
void	WebServ::shutdown()
{
	delete socketManager;
}
