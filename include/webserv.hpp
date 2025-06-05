/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:13 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/05 09:06:27 by jgraf            ###   ########.fr       */
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


//	Definition
typedef std::vector<std::string> t_vecstr;


//	Parsing
t_vecstr	read_config_file(std::string const &in_file);
bool		brace_check(const t_vecstr tokens);
bool		is_special_token(const std::string token);
bool		line_has_semicolon(const t_vecstr tokens, size_t i);