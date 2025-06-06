/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:13 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/05 14:58:00 by nmonzon          ###   ########.fr       */
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
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "exceptions.hpp"


//	Definition
typedef std::vector<std::string> t_vecstr;


//	Parsing
t_vecstr	read_config_file(std::string const &in_file);
