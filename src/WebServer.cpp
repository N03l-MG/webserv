/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:34:44 by jgraf             #+#    #+#             */
/*   Updated: 2025/05/28 11:41:23 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

//	Constructor
WebServer::WebServer()
{
	is_running = false;
}


//	Destructor
WebServer::~WebServer() {}


//	Start
void	WebServer::start(t_vecstr &tokens)
{
	is_running = true;
	this->tokens = tokens;
	parseConfig();
}


//	Parse Input
void	WebServer::parseConfig()
{
	int	block_depth = 0;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "{")
			block_depth ++;
		if (tokens[i] == "}")
			block_depth --;
		if (tokens[i] == "server"
		&& block_depth == 0)
			std::cout << "Entered server block" << std::endl;
	}
	if (block_depth != 0)
		throw ParseException();
}