/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:18:11 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/26 16:23:27 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <map>
#include "include.hpp"
#include "Location.hpp"

class	Location;

//	Class
class	Server
{
	private:
		std::string						host;			//the host to connect to
		int								port;			//the port to listen to
		std::string						name;			//the servers name
		std::string						root;			//the root repository
		std::string						index;			//the default page
		struct timeval					timeout;		//the max time select waits for a request
		size_t							max_body;		//max body size the client can recieve
		std::map<size_t, std::string>	error_page;		//List of error pages
		std::vector<Location*>			locations;		//list of page locations
		std::map<std::string, std::string> mimeTypes;
		struct HttpRequest
		{
			std::string method;
			std::string path;
			std::string version;
			std::map<std::string, std::string> headers;
			std::string body;
			std::string boundary;
		};

		HttpRequest parseRequest(const std::string& raw_request);
		std::string createResponse(int status_code, const std::string &content_type, const std::string &body);
		void handleGet(int client_fd, const HttpRequest& request);
		void handlePost(int client_fd, const HttpRequest& request);
		void handleDelete(int client_fd, const HttpRequest& request);
		void handleCgi(int client_fd, const HttpRequest& request);
		std::string getMimeType(const std::string &filepath);
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
		void		setTimeout(size_t timeout);
		void		setMaxBody(size_t max_body);
		void		addErrorpage(size_t code, std::string page);
		int			addLocation(Location *new_location);
		int			getPort();
		std::string	getHost();
		std::string	getName();
		std::string	getRoot();
		std::string	getIndex();
		struct timeval		getTimeout();
		size_t		getMaxBody();
		std::string						&getErrorpage(size_t code);
		std::map<size_t, std::string>	getErrorpage();
		Location				*getLocation(size_t index);
		std::vector<Location*>	getLocation();

		void	configure(t_vectok &tokens, size_t &i);
		void	print_status();
		void	respond(int client_fd, const std::string& request);
};
