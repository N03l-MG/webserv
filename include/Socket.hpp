/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:15:14 by nmonzon           #+#    #+#             */
/*   Updated: 2025/07/03 16:50:18 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
