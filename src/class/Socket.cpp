/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:14:48 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/23 10:41:33 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(Server *serv) : server(serv)
{
	log(LOG_LOG, "Socket created!");
	host = serv->getHost().c_str();
	port = serv->getPort();
	
	// Create socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		throw std::runtime_error("Failed to create socket");

	// Set socket options
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(server_fd);
		throw std::runtime_error("Failed to set socket options");
	}

	// Setup the socket
	if (setupSocket() != 0) {
		close(server_fd);
		throw std::runtime_error("Failed to setup socket");
	}
}

Socket::~Socket()
{
	log(LOG_LOG, "Socket destroyed!");
}

int Socket::setupSocket()
{
	sockaddr_in server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(host);

	if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Bind failed\n";
		close(server_fd);
		return 1;
	}

	int flags = fcntl(server_fd, F_GETFL, 0);
	fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

	if (listen(server_fd, SOMAXCONN) < 0) {
		std::cerr << "Listen failed\n";
		close(server_fd);
		return 1;
	}
	log(LOG_LOG, "Server listening on http://" + std::string(host) + ":" + std::to_string(port));
	return 0;
}
