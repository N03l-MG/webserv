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
#include "include.hpp"
#include "Location.hpp"

//	Class
class	Server
{
	private:
		std::string						host;			//the host to connect to
		int								port;			//the port to listen to
		std::string						name;			//the servers name
		std::string						root;			//the root repository
		std::string						index;			//the default page
		size_t							timeout;		//max response time for the server
		std::map<size_t, std::string>	error_page;		//List of error pages
		std::vector<Location*>			locations;		//list of page locations
	
	public:
		Server();
		~Server();

		void		setPort(int listen);
		void		setHost(std::string host);
		void		setName(std::string name);
		void		setRoot(std::string root);
		void		setIndex(std::string index);
		void		setTimeout(size_t time);
		void		addErrorpage(size_t cide, std::string page);
		int			addLocation(Location *new_location);
		int			getPort();
		std::string	getHost();
		std::string	getName();
		std::string	getRoot();
		std::string	getIndex();
		size_t		getTimeout();
		std::string						getErrorpage(size_t code);
		std::map<size_t, std::string>	getErrorpage();
		Location				*getLocation(size_t index);
		std::vector<Location*>	getLocation();

		void	configure(t_vectok &tokens, size_t &i);
		void	print_status();
};