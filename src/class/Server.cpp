/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:21:23 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/11 14:10:13 by nmonzon          ###   ########.fr       */
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

void Server::respond(int client_fd, const std::string& request)
{
	// Parse the path from the request line
	std::istringstream req_stream(request);
	std::string method, path, version;
	req_stream >> method >> path >> version;
	if (method == "POST" && path == "/upload") {
		handleUpload(client_fd, request);
		return;
	}
	if (path.empty() || path == "/")
		path = "/" + this->index; // default to index

	std::string filepath = this->root + path;

	std::ifstream file(filepath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		const std::string error_response =
			"HTTP/1.1 404 Not Found\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 13\r\n"
			"\r\n"
			"404 Not Found\r\n";
		send(client_fd, error_response.c_str(), error_response.size(), 0);
		return;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> file_buffer(size);
	file.read(file_buffer.data(), size);

	std::string content_type = getContentTypeFromExtension(filepath);

	std::string headers =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: " + content_type + "\r\n"
		"Content-Length: " + std::to_string(size) + "\r\n"
		"Connection: close\r\n"
		"\r\n";
	send(client_fd, headers.c_str(), headers.size(), 0);
	send(client_fd, file_buffer.data(), size, 0);
	file.close();
}

void Server::handleUpload(int client_fd, const std::string& request)
{
	// Find the start of the body
	size_t header_end = request.find("\r\n\r\n");
	if (header_end == std::string::npos) {
		// Malformed request
		return;
	}
	std::string headers = request.substr(0, header_end);
	std::string body = request.substr(header_end + 4);

	// Find boundary
	std::string boundary;
	size_t boundary_pos = headers.find("boundary=");
	if (boundary_pos != std::string::npos) {
		boundary = "--" + headers.substr(boundary_pos + 9);
		size_t end = boundary.find("\r");
		if (end != std::string::npos)
			boundary = boundary.substr(0, end);
	} else {
		// No boundary found
		return;
	}

	// Find file content
	size_t file_start = body.find("filename=\"");
	if (file_start == std::string::npos) return;
	file_start += 10;
	size_t file_end = body.find("\"", file_start);
	std::string filename = body.substr(file_start, file_end - file_start);

	// Find start of file data
	size_t data_start = body.find("\r\n\r\n", file_end);
	if (data_start == std::string::npos) return;
	data_start += 4;
	size_t data_end = body.find(boundary, data_start) - 4; // -4 to remove trailing \r\n--
	if (data_end == std::string::npos) return;

	std::string filedata = body.substr(data_start, data_end - data_start);

	// Save file
	std::ofstream outfile("www/uploads/" + filename, std::ios::binary);
	outfile.write(filedata.c_str(), filedata.size());
	outfile.close();

	// Respond
	std::string response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: 34\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<h1>Upload successful!</h1>\n";
	send(client_fd, response.c_str(), response.size(), 0);
}

std::string Server::getContentTypeFromExtension(const std::string &filepath)
{
	size_t dot_pos = filepath.find_last_of(".");
	if (dot_pos != std::string::npos) {
		std::string ext = filepath.substr(dot_pos);
		if (ext == ".html") return "text/html";
		if (ext == ".css") return "text/css";
		if (ext == ".js") return "application/javascript";
		if (ext == ".png") return "image/png";
		if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
		if (ext == ".gif") return "image/gif";
	}
	return "text/plain";
}
