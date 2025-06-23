/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
<<<<<<< HEAD
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:15:14 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/06 12:59:39 by nmonzon          ###   ########.fr       */
=======
<<<<<<< HEAD:include/WebServ.hpp
/*   webserv.hpp                                        :+:      :+:    :+:   */
=======
/*   Socket.hpp                                         :+:      :+:    :+:   */
>>>>>>> master:include/Socket.hpp
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:include/WebServ.hpp
/*   Created: 2025/05/28 08:32:39 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 12:22:42 by nmonzon          ###   ########.fr       */
=======
/*   Created: 2025/06/05 14:15:14 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/23 11:03:05 by jgraf            ###   ########.fr       */
>>>>>>> master:include/Socket.hpp
>>>>>>> master
/*                                                                            */
/* ************************************************************************** */

#pragma once

<<<<<<< HEAD
=======
<<<<<<< HEAD:include/WebServ.hpp
//	Include
#include "include.hpp"
=======
>>>>>>> master
#include <sys/select.h>
#include <fcntl.h>
#include <map>
#include <errno.h>
<<<<<<< HEAD
#include "Server.hpp"

class Socket
{
	private:
		int port;
		const char *host;
=======
>>>>>>> master:include/Socket.hpp
#include "Server.hpp"
#include "SocketManager.hpp"

<<<<<<< HEAD:include/WebServ.hpp
class	WebServ
{
	private:
		std::string				config_path;
		t_vecstr				tokens;
		bool					is_running;
		std::vector<Server*>	servers;
		SocketManager			*socketManager;

		void	parseConfig();

	public:
		WebServ();
		~WebServ();

		int		addServer(Server *new_server);
		Server	*getServer(size_t index);
		std::vector<Server*>	getServer();

		void	start(t_vecstr &tokens);
		void	shutdown();
		bool	isRunning() const { return is_running; }
=======
class Socket
{
	private:
		int			port;
		const char	*host;
	
>>>>>>> master
	public:
		int server_fd;
		Server *server;
		Socket(Server *serv);
		~Socket();

		int setupSocket();
<<<<<<< HEAD
=======
>>>>>>> master:include/Socket.hpp
>>>>>>> master
};
