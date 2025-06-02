/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:18:11 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/02 11:58:00 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include <iostream>
#include <string>
#include "webserv.hpp"

//	Class
class	ServerConfig
{
	private:
		int			listen;		//the port to listen to
		std::string	host;		//the host to connect to
		std::string	root;		//the root repository
		std::string	index;		//the default page
	
	public:
		ServerConfig();
		~ServerConfig();

		void		setListen(int listen);
		void		setHost(std::string host);
		void		setRoot(std::string root);
		void		setIndex(std::string index);
		int			getListen();
		std::string	getHost();
		std::string	getRoot();
		std::string	getIndex();

		void	configure(const t_vecstr &tokens, size_t &i);
};