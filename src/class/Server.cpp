/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:21:23 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 11:39:51 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//	Constructor
Server::Server()
{
	this->port = 0;
	this->host = "";
	this->name = "";
	this->root = "";
	this->index = "";
	this->autoindex = false;
}


//	Destructor
Server::~Server()
{
	//delete all locations to avoid leaking
	for (Location* loc : locations)
		delete loc;
	locations.clear();
}


//	Setter
void	Server::setPort(int port)
{
	this->port = port;
}

void	Server::setHost(std::string host)
{
	this->host = host;
}

void	Server::setName(std::string name)
{
	this->name = name;
}

void	Server::setRoot(std::string root)
{
	this->root = root;
}

void	Server::setIndex(std::string index)
{
	this->index = index;
}

void	Server::setAutoindex(bool autoindex)
{
	this->autoindex = autoindex;
}


//	Getter
int		Server::getPort()
{
	return (this->port);
}

std::string	Server::getHost()
{
	return (this->host);
}

std::string	Server::getName()
{
	return (this->name);
}

std::string	Server::getRoot()
{
	return (this->root);
}

std::string	Server::getIndex()
{
	return (this->index);
}

bool	Server::getAutoindex()
{
	return (this->autoindex);
}

Location	*Server::getLocation(size_t index)
{
	if (index < locations.size())
		return (locations[index]);
	return (NULL);
}

std::vector<Location*>	Server::getLocation()
{
	return (locations);
}


//	Add new location
int	Server::addLocation(Location *new_location)
{
	if (!new_location)
		return (1);
	locations.push_back(new_location);
	return (0);
}

//	Config
void	Server::configure(const t_vecstr &tokens, size_t &i)
{
	while (tokens[i] != "\0" && tokens[i] != "}")
	{
		if (tokens[i] == "listen")
			setPort(std::stoi(tokens[++i]));
		else if (tokens[i] == "host")
			setHost(tokens[++i]);
		else if (tokens[i] == "root")
			setRoot(tokens[++i]);
		else if (tokens[i] == "index")
			setIndex(tokens[++i]);
		else if (tokens[i] == "location")
		{
			Location	*new_location = new Location;
			if (addLocation(new_location) == 0)
				new_location->configure(tokens, i);
		}
		i ++;
	}
}

void	Server::run(int server_fd)
{
	while (true) {
		sockaddr_in client_addr{};
		socklen_t client_len = sizeof(client_addr);
		int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
		if (client_fd < 0) {
			std::cerr << "Failed to accept connection\n";
			continue;
		}

		FILE *client_stream = fdopen(client_fd, "r");
		if (!client_stream) {
			std::cerr << "Failed to create stream from socket\n";
			close(client_fd);
			continue;
		}

		std::string line;
		char buffer[1024];
		while (fgets(buffer, sizeof(buffer), client_stream)) {
			line = buffer;
			std::cout << line;
			if (line == "\n" || line == "\r\n")
				break;
		}
		respond(client_fd);

		fclose(client_stream);
	}
	close(server_fd);
}

void	Server::respond(int client_fd)
{
	// Construct the full path to index.html
	std::string filepath = this->root + this->index;
	std::ifstream file(filepath, std::ios::binary | std::ios::ate);
	
	if (!file.is_open()) {
		// If file cannot be opened, send 404
		const std::string error_response =
			"HTTP/1.1 404 Not Found\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 13\r\n"
			"\r\n"
			"404 Not Found\r\n";
		send(client_fd, error_response.c_str(), error_response.size(), 0);
		return;
	}

	// Get file size
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	// Read the entire file into a buffer
	std::vector<char> buffer(size);
	if (file.read(buffer.data(), size))
	{
		// Construct and send HTTP response headers
		std::string headers = 
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: " + std::to_string(size) + "\r\n"
			"Connection: close\r\n"
			"\r\n";
		
		// Send headers
		send(client_fd, headers.c_str(), headers.size(), 0);
		
		// Send file content
		send(client_fd, buffer.data(), size, 0);
	}
	file.close();
}
