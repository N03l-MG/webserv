/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:21:23 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/05 09:27:28 by jgraf            ###   ########.fr       */
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
	this->client_max_body_size = 1000000;
	this->autoindex = false;
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

void	Server::setAutoindex(bool autoindex)
{
	this->autoindex = autoindex;
}

void	Server::setBodysize(size_t size)
{
	this->client_max_body_size = size;
}

void	Server::addErrorpage(size_t code, std::string page)
{
	error_pages[code] = page;
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

size_t	Server::getBodysize()
{
	return (this->client_max_body_size);
}

std::string	Server::getErrorpage(size_t code)
{
	return (this->error_pages[code]);
}

std::map<size_t, std::string>	Server::getErrorpage()
{
	return (this->error_pages);
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
			else if (tokens[i] == "client_max_body_size")
				setBodysize(std::stoi(tokens[++i]));
			else if (tokens[i] == "error_page" && !is_special_token(tokens[i+2]))
			{
				addErrorpage(std::stoi(tokens[i+1]), tokens[i+2]);
				i += 2;
			}
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
			<< "Autoindex:\t" << getAutoindex() << "\n"
			<< "Body size:\t" << getBodysize() << std::endl;
	
	
		for (size_t i = 0; i < locations.size(); i++)
			std::cout << "Locations:\t" << getLocation(i) << std::endl;
		for (std::map<size_t, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
			std::cout << "Error Code:\t" << it->first  << "\t-> Page:\t" << it->second << std::endl;
}