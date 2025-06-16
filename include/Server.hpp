/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:18:11 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/16 16:01:14 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include "include.hpp"
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

		std::string createResponse(int status_code, const std::string &content_type, const std::string &body);
		void handleGet(int client_fd, std::string &path);
		void handlePost(int client_fd, const std::string &request);
		void handleDelete(int client_fd, const std::string &filepath);
		std::string getContentTypeFromExtension(const std::string &filepath);
		void handleCgi(int client_fd, const std::string &path, const std::string &method, const std::string &request);
		bool isCgiRequest(const std::string &path);
		std::string executeCgi(const std::string &script_path, const std::string &query_string, 
							const std::string &method, const std::string &body);

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
		void	respond(int client_fd, const std::string& request);
};
