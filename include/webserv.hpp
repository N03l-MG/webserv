/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:13 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/06 10:23:16 by jgraf            ###   ########.fr       */
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
#include "exceptions.hpp"


//	Definitions
typedef std::vector<std::string> t_vecstr;


//	Enums
typedef enum	e_toktype
{
	DIRECTIVE = 0,
	KEY = 1,
	OPEN_BRACE = 2,
	CLOSE_BRACE = 3,
	SEMICOLON = 4,
	VALUE = 5,
	END = 6
}	t_toktype;


//	Tokens
typedef struct s_tokens
{
	std::string	token;
	t_toktype	type;
}	t_tokens;


//	Parsing
std::vector<t_tokens>	read_config_file(std::string const &in_file);
void					tokenize(const std::string &line, std::vector<t_tokens> &tokens);
void					assign_token_type(std::vector<t_tokens> &tokens);
bool					brace_check(const t_vecstr tokens);
bool					is_special_token(const std::string token);
bool					has_semicolon(const t_vecstr tokens, size_t i);