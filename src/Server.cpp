/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:21:23 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/04 10:13:12 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//	Constructor
Server::Server()
{
	this->port = 0;
	this->host = "";
	this->name = "";
	this->root = "";
	this->index = "";
	this->autoindex = false;
}


//	Destructor
Server::~Server()
{
	//delete all locations to avoid leaking
	for (Location* loc : locations)
		delete loc;
	locations.clear();
}


//	Setter
void	Server::setPort(int port)
{
	this->port = port;
}

void	Server::setHost(std::string host)
{
	this->host = host;
}

void	Server::setName(std::string name)
{
	this->name = name;
}

void	Server::setRoot(std::string root)
{
	this->root = root;
}

void	Server::setIndex(std::string index)
{
	this->index = index;
}

void	Server::setAutoindex(bool autoindex)
{
	this->autoindex = autoindex;
}


//	Getter
int		Server::getPort()
{
	return (this->port);
}

std::string	Server::getHost()
{
	return (this->host);
}

std::string	Server::getName()
{
	return (this->name);
}

std::string	Server::getRoot()
{
	return (this->root);
}

std::string	Server::getIndex()
{
	return (this->index);
}

bool	Server::getAutoindex()
{
	return (this->autoindex);
}

Location	*Server::getLocation(size_t index)
{
	if (index < locations.size())
		return (locations[index]);
	return (NULL);
}

std::vector<Location*>	Server::getLocation()
{
	return (locations);
}


//	Add new location
int	Server::addLocation(Location *new_location)
{
	if (!new_location)
		return (1);
	locations.push_back(new_location);
	return (0);
}


//	Config
void	Server::configure(const t_vecstr &tokens, size_t &i)
{
	//check if all braces cleanly close
	if (!brace_check(tokens))
		throw ParseException();
	
	//configure
	while (tokens[i] != "\0" && tokens[i] != "}")
	{
		if (!is_special_token(tokens[i+1]) && line_has_semicolon(tokens, i))
		{
			if (tokens[i] == "listen")
				setPort(std::stoi(tokens[++i]));
			else if (tokens[i] == "host")
				setHost(tokens[++i]);
			else if (tokens[i] == "server_name")
				setName(tokens[++i]);
			else if (tokens[i] == "root")
				setRoot(tokens[++i]);
			else if (tokens[i] == "index")
				setIndex(tokens[++i]);
		}
		if (tokens[i] == "location" && !is_special_token(tokens[i+1]))
		{
			Location	*new_location = new Location;
			if (addLocation(new_location) == 0)
				new_location->configure(tokens, i);
		}
		i ++;
	}
	print_status();
}


//	Debug
void	Server::print_status()
{
	std::cout << "\n\t--- SERVER CONFIG ---" << std::endl;
	std::cout << "Port:\t\t" << getPort() << "\n"
			<< "Host:\t\t" << getHost() << "\n"
			<< "Name:\t\t" << getName() << "\n"
			<< "Root:\t\t" << getRoot() << "\n"
			<< "Index:\t\t" << getIndex() << "\n"
			<< "Autoindex:\t" << getAutoindex() << "\n" << std::endl;
	
	
	for (size_t i = 0; i < locations.size(); i++)
		std::cout << "Locations:\t" << getLocation(i) << std::endl;
}