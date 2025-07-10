#pragma once

#include "include.hpp"
#include "Server.hpp"
#include "SocketManager.hpp"

class	SocketManager;

class	WebServ
{
	private:
		bool					is_running;
		std::string				config_path;
		t_vectok				tokens;
		std::vector<Server*>	servers;
		SocketManager			*socketManager;

		void	parseConfig();

	public:
		WebServ();
		~WebServ();

		void		setTokens(t_vectok tokens);
		int			addServer(Server *new_server);
		t_tokens	*getToken(size_t index);
		t_vectok	getTokens();
		Server		*getServer(size_t index);
		std::vector<Server*>	getServer();
		void		rmServer(Server *server);

		void	start();
		void	shutdown();
		bool	isRunning();
};

extern WebServ	*g_webserver;
