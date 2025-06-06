/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:21:23 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 10:08:02 by jgraf            ###   ########.fr       */
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
}


//	Destructor
Server::~Server()
{
	//delete all locations to avoid leaking
	for (Location* loc : locations)
	{
		delete loc;
		std::cout << "Delete location" << std::endl;
	}
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

void	Server::addErrorpage(size_t code, std::string page)
{
	error_page[code] = page;
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

std::string	Server::getErrorpage(size_t code)
{
	return (this->error_page[code]);
}

std::map<size_t, std::string>	Server::getErrorpage()
{
	return (this->error_page);
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
	(void)i;
	//check if all braces cleanly close
	if (!brace_check(tokens))
		throw ParseException();
	
	//configure
	
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
			<< "Index:\t\t" << getIndex() << "\n" << std::endl;
	
	
		for (size_t i = 0; i < locations.size(); i++)
			std::cout << "Locations:\t" << getLocation(i) << std::endl;
		for (std::map<size_t, std::string>::iterator it = error_page.begin(); it != error_page.end(); it++)
			std::cout << "Error Code:\t" << it->first  << "\t-> Page:\t" << it->second << std::endl;
}