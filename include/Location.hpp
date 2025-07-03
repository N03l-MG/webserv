/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 08:49:46 by jgraf             #+#    #+#             */
/*   Updated: 2025/07/01 16:51:23 by jgraf            ###   ########.fr       */
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
		std::string	index;
		size_t		max_body;
		t_vecstr	cgi_scripts;
		t_vecstr	allow_methods;
		t_vecstr	_return;
	
	public:
		Location(Server *parent_server);
		~Location();

		void		setPath(std::string path);
		void		setIndex(std::string index);
		void		setMaxBody(size_t max_body);
		void		addMethod(std::string method);
		void		addCgi(std::string path);
		void		addReturn(std::string _return);
		std::string	getPath();
		std::string	getIndex();
		size_t		getMaxBody();
		std::string	getMethod(size_t index);
		t_vecstr	getMethod();
		std::string	getCgi(size_t index);
		t_vecstr	getCgi();
		std::string	getReturn(size_t index);
		t_vecstr	getReturn();

		void		configure(t_vectok &tokens, size_t &i);
		bool		checkMethod(std::string method);
		void		print_status();
};