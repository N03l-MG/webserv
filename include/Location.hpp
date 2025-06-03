/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 08:49:46 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/03 12:29:13 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include <iostream>
#include <string>
#include "webserv.hpp"

//	Class
class	Location
{
	private:
		std::string	path;
		std::string	root;
		std::string	index;
		std::string	_return;
		std::string	alias;
		bool		autoindex;
		t_vecstr	allow_methods;
		t_vecstr	cgi_path;
		t_vecstr	cgi_ext;

	public:
		Location();
		~Location();

		void		setPath(std::string path);
		void		setRoot(std::string root);
		void		setIndex(std::string index);
		void		setReturn(std::string _return);
		void		setAlias(std::string alias);
		void		setAutoindex(bool autoindex);
		void		addMethod(std::string method);
		void		addCgipath(std::string path);
		void		addCgiext(std::string ext);
		std::string	getPath();
		std::string	getRoot();
		std::string	getIndex();
		std::string	getReturn();
		std::string	getAlias();
		bool		getAutoindex();
		std::string	getMethod(size_t index);
		t_vecstr	getMethod();
		std::string	getCgipath(size_t index);
		t_vecstr	getCgipath();
		std::string	getCgiext(size_t index);
		t_vecstr	getCgiext();

		void	configure(const t_vecstr &tokens, size_t &i);
};
