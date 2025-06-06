/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:15:14 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/06 12:59:39 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/select.h>
#include <fcntl.h>
#include <map>
#include <errno.h>
#include "Server.hpp"

class Socket
{
	private:
		int port;
		const char *host;
	public:
		int server_fd;
		Server *server;
		Socket(Server *serv);
		~Socket();

		int setupSocket();
};
