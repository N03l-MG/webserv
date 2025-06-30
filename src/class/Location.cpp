/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 08:51:15 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/30 17:56:53 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

//	Constructor
Location::Location(Server *parent_server)
{
	log(LOG_INFO, "Location created!");
	this->path = "/";
	this->root = "";
	this->index = "";
	this->_return = "";
	this->alias = "";
	this->store = "";
	this->max_body = parent_server->getMaxBody();
}


//	Destructor
Location::~Location()
{
	log(LOG_INFO, "Location destroyed!");
}


//	Setters
void	Location::setPath(std::string path)			{ this->path = path; }
void	Location::setRoot(std::string root)			{ this->root = root; }
void	Location::setIndex(std::string index)		{ this->index = index; }
void	Location::setReturn(std::string _return)	{ this->_return = _return; }
void	Location::setAlias(std::string alias)		{ this->alias = alias; }
void	Location::setStore(std::string store)		{ this->store = store; }
void	Location::setMaxBody(size_t max_body)		{ this->max_body = max_body; }
void	Location::addMethod(std::string method)		{ this->allow_methods.push_back(method); }
void	Location::addCgipath(std::string path)		{ this->cgi_path.push_back(path); }


//	Getters
std::string	Location::getPath()		{ return (this->path); }
std::string	Location::getRoot()		{ return (this->root); }
std::string	Location::getIndex()	{ return (this->index); }
std::string	Location::getReturn()	{ return (this->_return); }
std::string	Location::getAlias()	{ return (this->alias); }
std::string	Location::getStore()	{ return (this->store); }
size_t		Location::getMaxBody()	{ return (this->max_body); }
t_vecstr	Location::getMethod()	{ return (this->allow_methods); }
t_vecstr	Location::getCgipath()	{ return (this->cgi_path); }
std::string	Location::getMethod(size_t index)
{
	if (index < allow_methods.size())
		return (allow_methods[index]);
	return ("");
}
std::string	Location::getCgipath(size_t index)
{
	if (index < cgi_path.size())
		return (cgi_path[index]);
	return ("");
}


//	Config
bool	Location::checkMethod(std::string method)
{
	if (method == "GET" || method == "POST" || method == "DELETE")
		return true;
	return false;
}

void	Location::configure(t_vectok &tokens, size_t &i)
{
	//	Set path
	while (tokens[i].type != TOK_OPEN_BRACE)
	{
		if (tokens[i].type == TOK_VALUE)
			setPath(tokens[i++].token);
	}
	
	//configure
	while (i + 1 < tokens.size() && tokens[++i].type != TOK_CLOSE_BRACE)
	{
		if (tokens[i].type == TOK_KEY)
		{
			std::string key = tokens[i].token;
			while (i + 1 < tokens.size() && tokens[++i].type != TOK_SEMICOLON && tokens[i].type != TOK_CLOSE_BRACE)
			{
				std::string value = tokens[i].token;
				if (key == "root")
					setRoot(value);
				else if (key == "index")
					setIndex(value);
				else if (key == "return")
					setReturn(value);
				else if (key == "alias")
					setAlias(value);
				else if (key == "upload_store")
					setStore(value);
				else if (key == "max_body")
					setMaxBody(std::stoi(value));
				else if (key == "max_body")
					setMaxBody(std::stoi(value));
				else if (key == "methods" && checkMethod(value))
					addMethod(value);
				else if (key == "cgi_path")
					addCgipath(value);
			}
		}
	}

	//print debug statement
	//print_status();
}


//	Debug
void	Location::print_status()
{
	std::cout << "\n\t--- LOCATION CONFIG ---" << std::endl;
	std::cout << "Path:\t\t" << getPath() << "\n"
			<< "Root:\t\t" << getRoot() << "\n"
			<< "Index:\t\t" << getIndex() << "\n"
			<< "Return:\t\t" << getReturn() << "\n"
			<< "Alias:\t\t" << getAlias() << "\n"
			<< "Upload Store:\t" << getStore() << "\n"
			<< "Body Size\t" << getMaxBody() << std::endl;
	
	
	for (size_t i = 0; i < allow_methods.size(); i++)
		std::cout << "Methods:\t" << getMethod(i) << std::endl;
	for (size_t i = 0; i < cgi_path.size(); i++)
		std::cout << "CGI Path:\t" << getCgipath(i) << std::endl;
}