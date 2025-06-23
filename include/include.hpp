/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
<<<<<<< HEAD
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<<< HEAD:include/include.hpp
/*   Created: 2025/05/13 16:28:13 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/05 14:58:00 by nmonzon          ###   ########.fr       */
========
/*   Created: 2025/05/28 08:32:39 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/23 10:59:17 by jgraf            ###   ########.fr       */
>>>>>>>> master:include/webserv.hpp
=======
/*   include.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:13 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/23 10:23:54 by jgraf            ###   ########.fr       */
>>>>>>> master
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
<<<<<<< HEAD
<<<<<<<< HEAD:include/include.hpp
=======
>>>>>>> master
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <vector>
<<<<<<< HEAD
=======
#include <map>
>>>>>>> master
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
<<<<<<< HEAD
#include "exceptions.hpp"
========
#include "include.hpp"
#include "Server.hpp"
#include "SocketManager.hpp"
>>>>>>>> master:include/webserv.hpp

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

<<<<<<<< HEAD:include/include.hpp
//	Parsing
t_vecstr	read_config_file(std::string const &in_file);
========
		void		setTokens(t_vectok tokens);
		int			addServer(Server *new_server);
		t_tokens	*getToken(size_t index);
		t_vectok	getTokens();
		Server		*getServer(size_t index);
		std::vector<Server*>	getServer();

		void	start();
		void	shutdown();
		bool	isRunning();
};
>>>>>>>> master:include/webserv.hpp
=======
#include <csignal>
#include "exceptions.hpp"


//	Enums
typedef enum	e_toktype
{
	TOK_DIRECTIVE,
	TOK_KEY,
	TOK_OPEN_BRACE,
	TOK_CLOSE_BRACE,
	TOK_SEMICOLON,
	TOK_VALUE,
	TOK_END
}	t_toktype;


typedef enum	e_logtype
{
	LOG_LOG,
	LOG_WARNING,
	LOG_ERROR
}	t_logtype;


//	Tokens
typedef struct s_tokens
{
	std::string	token;
	t_toktype	type;
}	t_tokens;


//	Definitions
typedef std::vector<std::string>	t_vecstr;
typedef std::vector<t_tokens>		t_vectok;


//	Parsing
t_vectok	read_config_file(std::string const &in_file);
void		tokenize(const std::string &line, t_vectok &tokens);
void		assign_token_type(t_vectok &tokens);


//	Logging
void	log(t_logtype type, std::string msg);
>>>>>>> master
