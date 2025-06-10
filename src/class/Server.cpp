/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:21:23 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 16:35:34 by nmonzon          ###   ########.fr       */
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

// void Server::handleGet(int client_fd, const std::string &path)
// {
// 	// Remove query parameters if present
// 	std::string clean_path = path;
// 	size_t query_pos = clean_path.find('?');
// 	if (query_pos != std::string::npos)
// 		clean_path = clean_path.substr(0, query_pos);

// 	// Construct filepath
// 	std::string filepath;
// 	if (clean_path == "/")
// 		filepath = this->root + this->index;
// 	else if (clean_path.find("/pages/") == 0)
// 		filepath = "www" + clean_path;
// 	else
// 		filepath = this->root + clean_path;

// 	std::ifstream file(filepath, std::ios::binary | std::ios::ate);
	
// 	if (!file.is_open()) {
// 		std::cerr << "Failed to open: " << filepath << std::endl;
// 		const std::string response = 
// 			"HTTP/1.1 404 Not Found\r\n"
// 			"Content-Type: text/plain\r\n"
// 			"Content-Length: 13\r\n"
// 			"\r\n"
// 			"404 Not Found\n";
// 		send(client_fd, response.c_str(), response.length(), 0);
// 		return;
// 	}

// 	// Get file size
// 	std::streamsize size = file.tellg();
// 	file.seekg(0, std::ios::beg);

// 	// Read file content
// 	std::vector<char> buffer(size);
// 	if (file.read(buffer.data(), size)) {
// 		std::string headers = 
// 			"HTTP/1.1 200 OK\r\n"
// 			"Content-Type: " + getContentTypeFromExtension(filepath) + "\r\n"
// 			"Content-Length: " + std::to_string(size) + "\r\n"
// 			"\r\n";
		
// 		send(client_fd, headers.c_str(), headers.length(), 0);
// 		send(client_fd, buffer.data(), size, 0);
// 	}
// 	file.close();
// }

// void Server::handlePost(int client_fd, const std::string &path, const std::string &body)
// {
// 	if (path == "/upload") {
// 		// Handle file upload
// 		std::string content_type = getContentType(body);;
// 		std::string boundary = getBoundary(content_type);
		
// 		try {
// 			saveUploadedFile(boundary, body);
// 			const std::string response = 
// 				"HTTP/1.1 201 Created\r\n"
// 				"Content-Type: text/plain\r\n"
// 				"Content-Length: 13\r\n"
// 				"\r\n"
// 				"File uploaded\n";
// 			send(client_fd, response.c_str(), response.length(), 0);
// 		} catch (const std::exception &e) {
// 			const std::string response = 
// 				"HTTP/1.1 500 Internal Server Error\r\n"
// 				"Content-Type: text/plain\r\n"
// 				"Content-Length: 21\r\n"
// 				"\r\n"
// 				"Upload failed: " + std::string(e.what());
// 			send(client_fd, response.c_str(), response.length(), 0);
// 		}
// 	}
// }

// std::string Server::getContentType(const std::string &request)
// {
// 	std::istringstream req_stream(request);
// 	std::string line;

// 	while (std::getline(req_stream, line))
// 		if (line.find("Content-Type: ") == 0)
// 			return line.substr(14);

// 	return "";
// }

// std::string Server::parseHttpRequest(const std::string &request, std::string &method, 
// 								std::string &path)
// {
// 	std::string content_length;
// 	std::istringstream req_stream(request);
// 	std::string line;

// 	// Parse first line for method and path
// 	if (std::getline(req_stream, line)) {
// 		std::istringstream line_stream(line);
// 		line_stream >> method >> path;
// 	}

// 	// Parse headers
// 	while (std::getline(req_stream, line) && line != "\r")
// 		if (line.find("Content-Length: ") == 0)
// 			content_length = line.substr(16);

// 	return content_length;
// }

// std::string Server::getBoundary(const std::string &content_type)
// {
// 	size_t pos = content_type.find("boundary=");
// 	if (pos != std::string::npos)
// 		return content_type.substr(pos + 9);
// 	return "";
// }

// void Server::saveUploadedFile(const std::string &boundary, const std::string &body)
// {
// 	// Find the filename in the content disposition header
// 	size_t filename_pos = body.find("filename=\"");
// 	if (filename_pos == std::string::npos)
// 		throw std::runtime_error("No filename found");
	
// 	filename_pos += 10;
// 	size_t filename_end = body.find("\"", filename_pos);
// 	std::string filename = body.substr(filename_pos, filename_end - filename_pos);

// 	// Find the start of file content
// 	size_t content_pos = body.find("\r\n\r\n", filename_end) + 4;
// 	size_t content_end = body.rfind(boundary) - 4;
	
// 	// Create uploads directory if it doesn't exist
// 	std::filesystem::create_directories("www/uploads");
	
// 	// Save the file
// 	std::ofstream outfile("www/uploads/" + filename, std::ios::binary);
// 	if (!outfile)
// 		throw std::runtime_error("Could not create file");
	
// 	outfile.write(body.c_str() + content_pos, content_end - content_pos);
// 	outfile.close();
// }
