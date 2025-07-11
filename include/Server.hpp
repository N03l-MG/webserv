#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <map>
#include "include.hpp"
#include "WebServ.hpp"
#include "Location.hpp"

class	Location;

class	Server
{
	private:
		std::string							host;			// the host to connect to
		int									port;			// the port to listen to
		std::string							name;			// the servers name
		std::string							root;			// the root repository
		std::string							index;			// the default page
		size_t								timeout;		// max response time for the server
		size_t								max_body;		// max body size the client can recieve
		std::map<size_t, std::string>		error_page;		// List of error pages
		std::vector<Location*>				locations;		// list of page locations
		std::map<std::string, std::string>	mimeTypes;		// list of mime types to be handled
		std::map<size_t, std::string>		statusCodes;	// list of codes to be returned as response
		time_t								last_active;	// keeps track of the last time the server was active

		struct HttpRequest {					// Parsed Request Data for use in handlers
			std::string							method;
			std::string							path;
			std::string							version;
			std::map<std::string, std::string>	headers;
			std::string							body;
			std::string							boundary;
			Location							*location;
		};

		// Private Methods
		void		percentDecode(std::string &body);
		std::string	CheckAlias(std::string path);
		std::string	normalizePath(const std::string &path);
		bool		isCgi(const HttpRequest &request);
		bool		checkMethods(const HttpRequest &request);
		void		handleGet(int client_fd, const HttpRequest &request);
		void		handlePost(int client_fd, const HttpRequest &request);
		void		handleDelete(int client_fd, const HttpRequest &request);
		void		handleCgi(int client_fd, const HttpRequest &request);
		std::string	executeCgi(const std::string &script_path, const std::string &query_string, const std::string &method, const std::string &body);
		void		saveFile(const std::string &filename, const std::string &file_content, int client_fd, const std::string &path);
		std::pair<std::string, std::string>	extractFileInfo(const HttpRequest &request);

	public:
		// Constructor & Destructor
		Server();
		~Server();

		// Setters & Getters
		void		setPort(int listen);
		void		setHost(std::string host);
		void		setName(std::string name);
		void		setRoot(std::string root);
		void		setIndex(std::string index);
		void		setTimeout(size_t time);
		void		setMaxBody(size_t max_body);
		void		addErrorpage(size_t code, std::string page);
		int			addLocation(Location *new_location);
		int			getPort();
		std::string	getHost();
		std::string	getName();
		std::string	getRoot();
		std::string	getIndex();
		size_t		getTimeout();
		size_t		getMaxBody();
		std::string	getMimeType(const std::string &filepath);
		std::string						&getErrorpage(size_t code);
		std::map<size_t, std::string>	getErrorpage();
		Location						*getLocation(size_t index);
		std::vector<Location*>			getLocation();

		// Public Methods
		void		configure(t_vectok &tokens, size_t &i);
		bool		braceCheck(t_vectok tokens);
		void		print_status();
		HttpRequest	parseRequest(const std::string &raw_request);
		void		respond(int client_fd, const std::string &request);
		std::string	createResponse(int status_code, const std::string &content_type, const std::string &body);
};
