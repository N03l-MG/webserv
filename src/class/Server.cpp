/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:21:23 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/16 15:24:20 by nmonzon          ###   ########.fr       */
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

void Server::respond(int client_fd, const std::string &request)
{
	// Parse the path from the request line
	std::istringstream req_stream(request);
	std::string method, path, version;
	req_stream >> method >> path >> version;
	if (method == "POST" && path == "/upload") {
		handleUpload(client_fd, request);
		return;
	}
	else if (method == "DELETE") {
		handleDelete(client_fd, path);
		return;
	}
	else
		handleGet(client_fd, path);
}

std::string Server::createResponse(int status_code, const std::string &content_type, const std::string &body)
{
	std::string status_text;
	switch (status_code) {
		case 200: status_text = "OK"; break;
		case 400: status_text = "Bad Request"; break;
		case 403: status_text = "Forbidden"; break;
		case 404: status_text = "Not Found"; break;
		case 405: status_text = "Method Not Allowed"; break;
		case 500: status_text = "Internal Server Error"; break;
		default: status_text = "Internal Server Error"; break;
	}

	return "HTTP/1.1 " + std::to_string(status_code) + " " + status_text + "\r\n"
		"Content-Type: " + content_type + "\r\n"
		"Content-Length: " + std::to_string(body.length()) + "\r\n"
		"Connection: close\r\n"
		"\r\n" +
		body;
}

void Server::handleGet(int client_fd, std::string &path)
{
	if (path.empty() || path == "/")
		path = "/" + this->index;

	std::string filepath = this->root + path;
	std::ifstream file(filepath, std::ios::binary | std::ios::ate);
	
	if (!file.is_open()) {
		std::string response = createResponse(404, "text/plain", "404 Not Found\r\n");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> file_buffer(size);
	file.read(file_buffer.data(), size);
	file.close();

	std::string content_type = getContentTypeFromExtension(filepath);

	std::string response = createResponse(200, content_type, std::string(file_buffer.data(), file_buffer.size()));
    send(client_fd, response.c_str(), response.size(), 0);
}

void Server::handleUpload(int client_fd, const std::string &request)
{
	// Find the start of the body
	size_t header_end = request.find("\r\n\r\n");
	if (header_end == std::string::npos) {
		std::string response = createResponse(400, "text/plain", "Malformed request\r\n");
		send(client_fd, response.c_str(), response.size(), 0);
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
		std::cout << "Found boundary: '" << boundary << "'" << std::endl;
	} else {
		std::cerr << "No boundary found in headers" << std::endl;
		return;
	}

	// Find file content
	size_t file_start = body.find("filename=\"");
	if (file_start == std::string::npos) {
		std::cerr << "No filename found" << std::endl;
		return;
	}
	file_start += 10;
	size_t file_end = body.find("\"", file_start);
	std::string filename = body.substr(file_start, file_end - file_start);
	std::cout << "Found filename: '" << filename << "'" << std::endl;

	// Find start of file data
	size_t data_start = body.find("\r\n\r\n", file_end);
	if (data_start == std::string::npos) {
		std::cerr << "No data start marker found" << std::endl;
		return;
	}
	data_start += 4;

	// Find the end boundary by looking for \r\n + boundary
	std::string boundary_marker = "\r\n" + boundary;
	size_t boundary_start = body.find(boundary_marker, data_start);
	if (boundary_start == std::string::npos) {
		// Try alternative boundary format
		boundary_marker = "\n" + boundary;
		boundary_start = body.find(boundary_marker, data_start);
		if (boundary_start == std::string::npos) {
			std::cerr << "No boundary found after data" << std::endl;
			return;
		}
	}

	// Calculate actual data size (excluding the \r\n before boundary)
	size_t data_end = boundary_start;
	std::cout << "Data size: " << (data_end - data_start) << " bytes" << std::endl;

	// Extract file data
	std::string filedata = body.substr(data_start, data_end - data_start);

	// Save file
	std::ofstream outfile("www/uploads/" + filename, std::ios::binary);
	if (!outfile.is_open()) {
		std::string response = createResponse(500, "text/plain", "Failed to create output file\r\n");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}

	outfile.write(filedata.c_str(), filedata.size());
	outfile.close();

	std::string response = createResponse(200, "text/html", "<h1>Upload successful!</h1>\n");
	send(client_fd, response.c_str(), response.size(), 0);
}

void Server::handleDelete(int client_fd, const std::string &filepath)
{
	if (filepath.substr(0, 8) != "/uploads/") {
		std::string response = createResponse(405, "text/plain", "Method Not Allowed\r\n");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}

	// Remove www/ from filepath if it exists
	std::string actual_path = filepath;
	if (actual_path.substr(0, 4) == "www/") {
		actual_path = actual_path.substr(4);
	}
	
	// Construct full path
	std::string full_path = "www/" + actual_path;
	
	if (std::filesystem::exists(full_path)) {
		if (std::remove(full_path.c_str()) == 0) {
			std::string response = createResponse(200, "text/plain", "File deleted\r\n");
			send(client_fd, response.c_str(), response.size(), 0);
		} else {
			std::string response = createResponse(403, "text/plain", "Failed to delete file\r\n");
			send(client_fd, response.c_str(), response.size(), 0);
		}
	} else {
		std::string response = createResponse(404, "text/plain", "404 Not Found\r\n");
		send(client_fd, response.c_str(), response.size(), 0);
	}
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
		if (ext == ".md") return "text/markdown";
	}
	return "text/plain";
}
