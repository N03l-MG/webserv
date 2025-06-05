/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:15:14 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/05 16:15:19 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

class Socket
{
	private:
		int port;
		const char *host;
	public:
		int server_fd;
		Socket(Server *serv);
		~Socket();

		int setupSocket();
};
