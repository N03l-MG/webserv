/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 08:49:46 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/27 14:57:06 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//	Guard
#pragma once

//	Include
#include <iostream>
#include <string>
#include "include.hpp"
#include "Server.hpp"

class	Server;
#include "Server.hpp"

class	Server;

//	Class
class	Location
{
	private:
		std::string	path;
		std::string	root;
		std::string	index;
		std::string	_return;
		std::string	alias;
		std::string	store;
		size_t		max_body;
		t_vecstr	cgi_path;
		std::vector<t_methods>	allow_methods;
	
	public:
		Location(Server *parent_server);
		Location(Server *parent_server);
		~Location();

		void		setPath(std::string path);
		void		setRoot(std::string root);
		void		setIndex(std::string index);
		void		setReturn(std::string _return);
		void		setAlias(std::string alias);
		void		setStore(std::string store);
		void		setMaxBody(size_t max_body);
		void		addMethod(t_methods method);
		void		addCgipath(std::string path);
		std::string	getPath();
		std::string	getRoot();
		std::string	getIndex();
		std::string	getReturn();
		std::string	getAlias();
		std::string	getStore();
		size_t		getMaxBody();
		t_methods	getMethod(size_t index);
		std::vector<t_methods>	getMethod();
		std::string	getCgipath(size_t index);
		t_vecstr	getCgipath();

		void		configure(t_vectok &tokens, size_t &i);
		t_methods	strToMeth(std::string method);
		void		print_status();
};