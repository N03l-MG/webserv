#include "Socket.hpp"
#include "SocketManager.hpp"

//	Constructor
Socket::Socket(Server *serv)
{
	log(LOG_INFO, "Socket created!");
	server = serv;
	host = serv->getHost().c_str();
	port = serv->getPort();
	
	//create socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		throw	std::runtime_error("Failed to create socket.");

	//set socket options
	int	opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		throw	std::runtime_error("Failed to set socket options.");
		close(server_fd);
	}

	//setup the socket
	if (setupSocket() != 0)
	{
		throw	std::runtime_error("Failed to setup socket.");
		close(server_fd);
	}
}


//	Destructor
Socket::~Socket()
{
	log(LOG_INFO, "Socket destroyed!");
}


//	Socket setup
int	Socket::setupSocket()
{
	sockaddr_in	server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(host);

	if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		log(LOG_ERR, "Bind failed: " + std::string(strerror(errno)));
		close(server_fd);
		return 1;
	}

	if (listen(server_fd, SOMAXCONN) < 0)
	{
		log(LOG_ERR, "Failed to listen.");
		close(server_fd);
		return 1;
	}
	log(LOG_INFO, "Server listening on http://" + std::string(host) + ":" + std::to_string(port));
	return 0;
}
