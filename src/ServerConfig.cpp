/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:21:23 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/02 13:00:47 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

//	Constructor
ServerConfig::ServerConfig() {}


//	Destructor
ServerConfig::~ServerConfig() {}


//	Setter
void	ServerConfig::setListen(int listen)
{
	this->listen = listen;
}

void	ServerConfig::setHost(std::string host)
{
	this->host = host;
}

void	ServerConfig::setRoot(std::string root)
{
	this->root = root;
}

void	ServerConfig::setIndex(std::string index)
{
	this->index = index;
}


//	Getter
int		ServerConfig::getListen()
{
	return (this->listen);
}

std::string	ServerConfig::getHost()
{
	return (this->host);
}

std::string	ServerConfig::getRoot()
{
	return (this->root);
}

std::string	ServerConfig::getIndex()
{
	return (this->index);
}


//	Config
void	ServerConfig::configure(const t_vecstr &tokens, size_t &i)
{
	while (tokens[i] != "\0" && tokens[i] != "}")
	{
		if (tokens[i] == "listen")
			setListen(std::stoi(tokens[++i]));
		else if (tokens[i] == "host")
			setHost(tokens[++i]);
		else if (tokens[i] == "root")
			setRoot(tokens[++i]);
		else if (tokens[i] == "index")
			setIndex(tokens[++i]);
		i ++;
	}
}