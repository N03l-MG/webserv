#pragma once

#include <sys/select.h>
#include <fcntl.h>
#include <map>
#include <errno.h>
#include "WebServ.hpp"
#include "Server.hpp"

class	Server;

class	Socket
{
	private:
		int			port;
		const char	*host;
	
	public:
		Socket(Server *serv);
		~Socket();

		int		server_fd;
		Server	*server;

		int	setupSocket();
};
