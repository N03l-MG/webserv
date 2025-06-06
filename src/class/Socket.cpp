/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:14:48 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/06 13:00:22 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(Server *serv) : server(serv)
{
	host = serv->getHost().c_str();
	port = serv->getPort();
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd == -1) {
		std::cerr << "Failed to create socket\n";
		return;
	}
	if (setupSocket() != 0)
		exit(1); // FIXME: bullshit way to handle this
}

Socket::~Socket() {}

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
	std::cout << "Server listening on http://" << host << ":" << port << std::endl;
	return 0;
}
