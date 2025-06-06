/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:13 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/06 12:59:56 by jgraf            ###   ########.fr       */
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


//	Enums
typedef enum	e_toktype
{
	TOK_DIRECTIVE = 0,
	TOK_KEY = 1,
	TOK_OPEN_BRACE = 2,
	TOK_CLOSE_BRACE = 3,
	TOK_SEMICOLON = 4,
	TOK_VALUE = 5,
	TOK_END = 6
}	t_toktype;


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