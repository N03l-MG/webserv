/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 08:51:15 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 10:10:36 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

//	Constructor
Location::Location()
{
	this->path = "";
	this->root = "";
	this->index = "";
	this->_return = "";
	this->alias = "";
}


//	Destructor
Location::~Location() {}


//	Setters
void		Location::setPath(std::string path)
{
	if (!is_special_token(path))
		this->path = path;
}

void		Location::setRoot(std::string root)
{
	if (!is_special_token(root))
		this->root = root;
}

void		Location::setIndex(std::string index)
{
	if (!is_special_token(index))
		this->index = index;
}

void		Location::setReturn(std::string _return)
{
	if (!is_special_token(_return))
		this->_return = _return;
}

void		Location::setAlias(std::string alias)
{
	if (!is_special_token(alias))
		this->alias = alias;
}

void		Location::addMethod(std::string method)
{
	if (!is_special_token(method))
		this->allow_methods.push_back(method);
}

void		Location::addCgipath(std::string path)
{
	if (!is_special_token(path))
		this->cgi_path.push_back(path);
}


//	Getters
std::string	Location::getPath()
{
	return (this->path);
}

std::string	Location::getRoot()
{
	return (this->root);
}

std::string	Location::getIndex()
{
	return (this->index);
}

std::string	Location::getReturn()
{
	return (this->_return);
}

std::string	Location::getAlias()
{
	return (this->alias);
}

std::string	Location::getMethod(size_t index)
{
	if (index < allow_methods.size())
		return (allow_methods[index]);
	return ("");
}

t_vecstr	Location::getMethod()
{
	return (this->allow_methods);
}

std::string	Location::getCgipath(size_t index)
{
	if (index < cgi_path.size())
		return (cgi_path[index]);
	return ("");
}

t_vecstr	Location::getCgipath()
{
	return (this->cgi_path);
}


//	Config
void Location::configure(const t_vecstr &tokens, size_t &i)
{
	(void)tokens;
	(void)i;
	//move to start of location block and get path
	/*setPath(tokens[++i]);
	if (tokens[++i] != "{")
		throw ParseException();

	//define a map of configuration keywords to lambdas
	std::map<std::string, std::function<void()>> configMap = {
		{"path", [&]() { while (tokens[i+1] != ";" && tokens[i+1] != "\0" && tokens[i+1] != "}") setPath(tokens[++i]); }},
		{"root", [&]() { while (tokens[i+1] != ";" && tokens[i+1] != "\0" && tokens[i+1] != "}") setRoot(tokens[++i]); }},
		{"index", [&]() { while (tokens[i+1] != ";" && tokens[i+1] != "\0" && tokens[i+1] != "}") setIndex(tokens[++i]); }},
		{"return", [&]() { while (tokens[i+1] != ";" && tokens[i+1] != "\0" && tokens[i+1] != "}") setReturn(tokens[++i]); }},
		{"alias", [&]() { while (tokens[i+1] != ";" && tokens[i+1] != "\0" && tokens[i+1] != "}") setAlias(tokens[++i]); }},
		{"autoindex", [&]() { while (tokens[i+1] != ";" && tokens[i+1] != "\0" && tokens[i+1] != "}") setAutoindex(tokens[++i] == "on"); }},
		{"allow_methods", [&]() { while (tokens[i+1] != ";" && tokens[i+1] != "\0" && tokens[i+1] != "}") addMethod(tokens[++i]); }},
		{"cgi_path", [&]() { while (tokens[i+1] != ";" && tokens[i+1] != "\0" && tokens[i+1] != "}") addCgipath(tokens[++i]); }},
		{"cgi_ext", [&]() { while (tokens[i+1] != ";" && tokens[i+1] != "\0" && tokens[i+1] != "}") addCgiext(tokens[++i]); }}
	};

	//process tokens
	while (tokens[i] != "\0" && tokens[i] != "}")
	{
		if (configMap.find(tokens[i]) != configMap.end())
			configMap[tokens[i]]();
		else
			i ++;
	}*/

	//print debug statement
	print_status();
}


//	Debug
void	Location::print_status()
{
	std::cout << "\n\t--- LOCATION CONFIG ---" << std::endl;
	std::cout << "Path:\t\t" << getPath() << "\n"
			<< "Root:\t\t" << getRoot() << "\n"
			<< "Index:\t\t" << getIndex() << "\n"
			<< "Return:\t\t" << getReturn() << "\n"
			<< "Alias:\t\t" << getAlias() << std::endl;
	
	
	for (size_t i = 0; i < allow_methods.size(); i++)
		std::cout << "Methods:\t" << getMethod(i) << std::endl;
	for (size_t i = 0; i < cgi_path.size(); i++)
		std::cout << "CGI Path:\t" << getCgipath(i) << std::endl;
}