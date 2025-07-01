/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 08:49:46 by jgraf             #+#    #+#             */
/*   Updated: 2025/07/01 10:21:37 by jgraf            ###   ########.fr       */
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
		t_vecstr	cgi_scripts;
		t_vecstr	allow_methods;
	
	public:
		Location(Server *parent_server);
		~Location();

		void		setPath(std::string path);
		void		setRoot(std::string root);
		void		setIndex(std::string index);
		void		setReturn(std::string _return);
		void		setAlias(std::string alias);
		void		setStore(std::string store);
		void		setMaxBody(size_t max_body);
		void		addMethod(std::string method);
		void		addCgi(std::string path);
		std::string	getPath();
		std::string	getRoot();
		std::string	getIndex();
		std::string	getReturn();
		std::string	getAlias();
		std::string	getStore();
		size_t		getMaxBody();
		std::string	getMethod(size_t index);
		t_vecstr	getMethod();
		std::string	getCgi(size_t index);
		t_vecstr	getCgi();

		void		configure(t_vectok &tokens, size_t &i);
		bool		checkMethod(std::string method);
		void		print_status();
};