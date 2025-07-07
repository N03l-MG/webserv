/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:13 by nmonzon           #+#    #+#             */
/*   Updated: 2025/07/07 15:50:55 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <vector>
#include <map>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <csignal>
#include <poll.h>
#include <ctime>
#include <fcntl.h>


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
	LOG_INFO,
	LOG_WARN,
	LOG_ERR
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
t_vectok	read_config_file(int ac, std::string const &in_file);
void		tokenize(const std::string &line, t_vectok &tokens);
void		assign_token_type(t_vectok &tokens);


//	Logging
void	log(t_logtype type, std::string msg);