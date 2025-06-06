/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 08:49:46 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/05 15:07:52 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include <iostream>
#include <string>
#include <map>
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
		t_vecstr	allow_methods;
		t_vecstr	cgi_path;
	
	public:
		Location();
		~Location();

		void		setPath(std::string path);
		void		setRoot(std::string root);
		void		setIndex(std::string index);
		void		setReturn(std::string _return);
		void		setAlias(std::string alias);
		void		addMethod(std::string method);
		void		addCgipath(std::string path);
		std::string	getPath();
		std::string	getRoot();
		std::string	getIndex();
		std::string	getReturn();
		std::string	getAlias();
		std::string	getMethod(size_t index);
		t_vecstr	getMethod();
		std::string	getCgipath(size_t index);
		t_vecstr	getCgipath();

		void	configure(const t_vecstr &tokens, size_t &i);
		void	print_status();
};