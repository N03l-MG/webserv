/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 08:51:15 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/03 12:18:02 by jgraf            ###   ########.fr       */
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
	this->autoindex = false;
}


//	Destructor
Location::~Location() {}


//	Setters
void		Location::setPath(std::string path)
{
	this->path = path;
}

void		Location::setRoot(std::string root)
{
	this->root = root;
}

void		Location::setIndex(std::string index)
{
	this->index = index;
}

void		Location::setReturn(std::string _return)
{
	this->_return = _return;
}

void		Location::setAlias(std::string alias)
{
	this->alias = alias;
}

void		Location::setAutoindex(bool autoindex)
{
	this->autoindex = autoindex;
}

void		Location::addMethod(std::string method)
{
	this->allow_methods.push_back(method);
}

void		Location::addCgipath(std::string path)
{
	this->cgi_path.push_back(path);
}

void		Location::addCgiext(std::string ext)
{
	this->cgi_ext.push_back(ext);
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

bool		Location::getAutoindex()
{
	return (this->autoindex);
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

std::string	Location::getCgiext(size_t index)
{
	if (index < cgi_ext.size())
		return (cgi_ext[index]);
	return ("");
}

t_vecstr	Location::getCgiext()
{
	return (this->cgi_ext);
}


//	Config
void	Location::configure(const t_vecstr &tokens, size_t &i)
{
	while (tokens[i] != "\0" && tokens[i] != "}")
	{
		i ++;
	}
}