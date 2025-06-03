/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:18:11 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/03 09:07:20 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include <iostream>
#include <string>
#include "webserv.hpp"
#include "Location.hpp"

//	Class
class	Server
{
	private:
		int						port;		//the port to listen to
		std::string				host;		//the host to connect to
		std::string				name;		//the servers name
		std::string				root;		//the root repository
		std::string				index;		//the default page
		bool					autoindex;	//autoindex
		std::vector<Location*>	locations;	//list of page locations
	
	public:
		Server();
		~Server();

		void		setPort(int listen);
		void		setHost(std::string host);
		void		setName(std::string name);
		void		setRoot(std::string root);
		void		setIndex(std::string index);
		void		setAutoindex(bool autoindex);
		int			addLocation(Location *new_location);
		int			getPort();
		std::string	getHost();
		std::string	getName();
		std::string	getRoot();
		std::string	getIndex();
		bool		getAutoindex();
		Location	*getLocation(size_t index);
		std::vector<Location*>	getLocation();

		void	configure(const t_vecstr &tokens, size_t &i);
};