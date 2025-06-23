/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 08:49:46 by jgraf             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/06 12:32:04 by nmonzon          ###   ########.fr       */
=======
/*   Updated: 2025/06/23 14:20:01 by jgraf            ###   ########.fr       */
>>>>>>> master
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include <iostream>
#include <string>
#include "include.hpp"

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
		void		setStore(std::string store);
		void		addMethod(std::string method);
		void		addCgipath(std::string path);
		std::string	getPath();
		std::string	getRoot();
		std::string	getIndex();
		std::string	getReturn();
		std::string	getAlias();
		std::string	getStore();
		std::string	getMethod(size_t index);
		t_vecstr	getMethod();
		std::string	getCgipath(size_t index);
		t_vecstr	getCgipath();

<<<<<<< HEAD
		void	configure(const t_vecstr &tokens, size_t &i);
};
=======
		void	configure(t_vectok &tokens, size_t &i);
		void	print_status();
};
>>>>>>> master
