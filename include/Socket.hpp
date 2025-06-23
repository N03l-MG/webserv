/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:15:14 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/23 11:03:05 by jgraf            ###   ########.fr       */
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
		int			port;
		const char	*host;
	
	public:
		int server_fd;
		Server *server;
		Socket(Server *serv);
		~Socket();

		int setupSocket();
};
