/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:21:23 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/24 16:25:26 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//	Constructor
Server::Server()
{
	log(LOG_LOG, "Server created!");
	this->port = 0;
	this->host = "";
	this->name = "";
	this->root = "";
	this->index = "";

	// MIME types
	mimeTypes[".html"] = "text/html";
	mimeTypes[".htm"] = "text/html";
	mimeTypes[".css"] = "text/css";
	mimeTypes[".js"] = "application/javascript";
	mimeTypes[".json"] = "application/json";
	mimeTypes[".xml"] = "application/xml";
	mimeTypes[".txt"] = "text/plain";
	mimeTypes[".md"] = "text/markdown";
	mimeTypes[".jpg"] = "image/jpeg";
	mimeTypes[".jpeg"] = "image/jpeg";
	mimeTypes[".png"] = "image/png";
	mimeTypes[".gif"] = "image/gif";
	mimeTypes[".svg"] = "image/svg+xml";
	mimeTypes[".ico"] = "image/x-icon";
	mimeTypes[".mp3"] = "audio/mpeg";
	mimeTypes[".wav"] = "audio/wav";
	mimeTypes[".mp4"] = "video/mp4";
	mimeTypes[".webm"] = "video/webm";
	mimeTypes[".pdf"] = "application/pdf";
	mimeTypes[".doc"] = "application/msword";
	mimeTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mimeTypes[".zip"] = "application/zip";
	mimeTypes[".gz"] = "application/gzip";
	mimeTypes[".tar"] = "application/x-tar";
}

//	Destructor
Server::~Server()
{
	//delete all locations to avoid leaking
	for (Location *loc : locations)
		delete loc;
	locations.clear();
	error_page.clear();
	log(LOG_LOG, "Server destroyed!");
}

//	Setters
void	Server::setPort(int port) { this->port = port; }
void	Server::setHost(std::string host) { this->host = host; }
void	Server::setName(std::string name) { this->name = name; }
void	Server::setRoot(std::string root) { this->root = root; }
void	Server::setIndex(std::string index) { this->index = index; }
void	Server::setTimeout(size_t timeout) { this->timeout = timeout; }
void	Server::addErrorpage(size_t code, std::string page) { error_page[code] = page; }
int		Server::addLocation(Location *new_location)
{
	if (!new_location)
		return (1);
	locations.push_back(new_location);
	return (0);
}

//	Getters
int			Server::getPort() { return (this->port); }
std::string	Server::getHost() { return (this->host); }
std::string	Server::getName() { return (this->name); }
std::string	Server::getRoot() { return (this->root); }
std::string	Server::getIndex() { return (this->index); }
size_t		Server::getTimeout() { return (this->timeout); }
std::vector<Location*>	Server::getLocation() { return (locations); }
std::map<size_t, std::string>	Server::getErrorpage() { return (error_page); }
std::string	&Server::getErrorpage(size_t code) { return (this->error_page[code]); }
Location	*Server::getLocation(size_t index)
{
	if (index < locations.size())
		return (locations[index]);
	return (NULL);
}

// ================================================= //
// ===========         CONFIGURE         =========== //
// ================================================= //

static bool	brace_check(t_vectok tokens)
{
	int	cnt = 0;

	for (size_t	i = 0; i < tokens.size() ; i++)
	{
		if (tokens[i].type == TOK_OPEN_BRACE)
			cnt ++;
		if (tokens[i].type == TOK_CLOSE_BRACE)
			cnt --;
	}
	if (cnt == 0)
		return (true);
	return (false);
}

void	Server::configure(t_vectok &tokens, size_t &i)
{
	std::string	key;
	std::string	value;

	//check if all braces cleanly close
	if (!brace_check(tokens) || tokens[i].type != TOK_OPEN_BRACE)
		throw ParseException();

	//configure
	while (tokens[++i].type != TOK_CLOSE_BRACE)
	{
		if (tokens[i].type == TOK_KEY)
		{
			key = tokens[i++].token;
			value = tokens[i].token;
			if (tokens[i].type != TOK_VALUE)
				throw ParseException();

			if (key == "host")
				setHost(value);
			else if (key == "listen")
				setPort(std::stoi(value));
			else if (key == "server_name")
				setName(value);
			else if (key == "root")
				setRoot(value);
			else if (key == "index")
				setIndex(value);
			else if (key == "timeout")
				setTimeout(std::stoi(value));
			else if (key == "error_page")
			{
				if (tokens[++i].type != TOK_VALUE) // Ensure the next token is a value
					throw ParseException();
				std::string page = tokens[i].token;
				addErrorpage(std::stoi(value), page);
			}
		}
		else if (tokens[i].type == TOK_DIRECTIVE && tokens[i].token == "location")
		{
			Location *new_location = new Location;
			new_location->configure(tokens, ++i);
			locations.push_back(new_location);
		}
	}

	//print data
	//print_status();
}

//	Debug
void	Server::print_status()
{
	std::cout << "\n\t--- SERVER CONFIG ---" << std::endl;
	std::cout << "Port:\t\t" << getPort() << "\n"
			<< "Host:\t\t" << getHost() << "\n"
			<< "Name:\t\t" << getName() << "\n"
			<< "Root:\t\t" << getRoot() << "\n"
			<< "Index:\t\t" << getIndex() << "\n" << std::endl;
	
	for (size_t i = 0; i < locations.size(); i++)
		std::cout << "Locations:\t" << getLocation(i) << std::endl;
	for (std::map<size_t, std::string>::iterator it = error_page.begin(); it != error_page.end(); it++)
		std::cout << "Error Code:\t" << it->first  << "\t-> Page:\t" << it->second << std::endl;
}

// ================================================= //
// ===========       HTTP RESPONSE       =========== //
// ================================================= //

Server::HttpRequest Server::parseRequest(const std::string& raw_request) 
{
	HttpRequest request;
	std::istringstream request_stream(raw_request);

	std::string request_line;
	std::getline(request_stream, request_line);
	std::istringstream request_line_stream(request_line);
	request_line_stream >> request.method >> request.path >> request.version;

	std::string header_line;
	while (std::getline(request_stream, header_line) && header_line != "\r") {
		size_t colon_pos = header_line.find(':');
		if (colon_pos != std::string::npos) {
			std::string key = header_line.substr(0, colon_pos);
			std::string value = header_line.substr(colon_pos + 2); // Skip ": "
			if (!value.empty() && value[value.length()-1] == '\r') {
				value = value.substr(0, value.length()-1);
			}
			request.headers[key] = value;
		}
	}

	auto content_type_it = request.headers.find("Content-Type");
	if (content_type_it != request.headers.end()) {
		size_t boundary_pos = content_type_it->second.find("boundary=");
		if (boundary_pos != std::string::npos) {
			request.boundary = content_type_it->second.substr(boundary_pos + 9);
		}
	}

	size_t header_end = raw_request.find("\r\n\r\n");
	if (header_end != std::string::npos) {
		request.body = raw_request.substr(header_end + 4);
	}
	
	return request;
}

// Modify your respond method to use the parser:
void Server::respond(int client_fd, const std::string& raw_request)
{
	HttpRequest request = parseRequest(raw_request);
	
	if (isCgiRequest(request.path))
		handleCgi(client_fd, request);
	else if (request.method == "POST" && request.path == "/upload")
		handlePost(client_fd, request);
	else if (request.method == "DELETE")
		handleDelete(client_fd, request);
	else
		handleGet(client_fd, request);
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

// ================================================= //
// ===========      METHOD HANDLING      =========== //
// ================================================= //

void Server::handleGet(int client_fd, const HttpRequest& request)
{
	std::string path = request.path;
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

	std::string content_type = getMimeType(filepath);

	std::string response = createResponse(200, content_type, std::string(file_buffer.data(), file_buffer.size()));
    send(client_fd, response.c_str(), response.size(), 0);
}

void Server::handlePost(int client_fd, const HttpRequest& request)
{
	if (request.boundary.empty()) {
		std::string response = createResponse(400, "text/plain", "Bad Request: No boundary found\n");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}
	
	// Find filename in the body
	size_t filename_pos = request.body.find("filename=\"");
	if (filename_pos == std::string::npos) {
		std::string response = createResponse(400, "text/plain", "Bad Request: No filename found\n");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}
	
	// Extract filename
	filename_pos += 10;
	size_t filename_end = request.body.find("\"", filename_pos);
	std::string filename = request.body.substr(filename_pos, filename_end - filename_pos);
	
	// Find file content
	size_t content_start = request.body.find("\r\n\r\n", filename_end) + 4;
	size_t content_end = request.body.find("--" + request.boundary + "--") - 2;
	
	if (content_start == std::string::npos || content_end == std::string::npos) {
		std::string response = createResponse(400, "text/plain", "Bad Request: Invalid file content\n");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}
	
	// Extract file content
	std::string file_content = request.body.substr(content_start, content_end - content_start);
	
	// Create directory and save file
	std::string upload_dir = "www/uploads";
	if (!std::filesystem::exists(upload_dir)) {
		if (!std::filesystem::create_directory(upload_dir)) {
			std::string response = createResponse(500, "text/plain", "Server Error: Failed to create upload directory\n");
			send(client_fd, response.c_str(), response.size(), 0);
			return;
		}
	}
	
	std::string filepath = upload_dir + "/" + filename;
	std::ofstream outfile(filepath, std::ios::binary);
	if (!outfile.is_open()) {
		std::string response = createResponse(500, "text/plain", "Server Error: Failed to create file\n");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}
	
	outfile.write(file_content.c_str(), file_content.length());
	outfile.close();
	std::string response = createResponse(200, "text/html", 
		"<h1>Upload successful!</h1>\n"
		"<p>File '" + filename + "' has been uploaded.</p>\n");
	send(client_fd, response.c_str(), response.size(), 0);
}

void Server::handleDelete(int client_fd, const HttpRequest& request) // TODO: Fix massive security issues.
{
	if (request.path.substr(0, 8) != "/uploads/") {
        std::string response = createResponse(405, "text/plain", "Method Not Allowed\r\n");
        send(client_fd, response.c_str(), response.size(), 0);
        return;
    }

	// Remove www/ from filepath if it exists
	std::string actual_path = request.path;
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

std::string Server::getMimeType(const std::string &filepath)
{
	size_t dot_pos = filepath.find_last_of(".");
	if (dot_pos != std::string::npos) {
		std::string ext = filepath.substr(dot_pos);
		auto it = mimeTypes.find(ext);
		if (it != mimeTypes.end()) {
			return it->second;
		}
	}
	return "application/octet-stream"; // Unknown file default
}

// ================================================= //
// ===========       CGI REQUESTS        =========== //
// ================================================= //

bool Server::isCgiRequest(const std::string &path)
{
	// Check if path starts with /cgi-bin/
	return path.substr(0, 9) == "/cgi-bin/";
}

void Server::handleCgi(int client_fd, const HttpRequest& request)
{
	std::string script_path = this->root + request.path;
	std::string query_string;
	
	try {
		std::string output = executeCgi(script_path, query_string, request.method, request.body);
		std::string response = createResponse(200, "text/html", output);
		send(client_fd, response.c_str(), response.size(), 0);
	} catch (const std::exception &e) {
		std::string response = createResponse(500, "text/plain", "CGI execution failed\r\n");
		send(client_fd, response.c_str(), response.size(), 0);
	}
}

std::string Server::executeCgi(const std::string &script_path, const std::string &query_string,
							const std::string &method, const std::string &body)
{
	// Set up environment variables
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("REQUEST_METHOD", method.c_str(), 1);
	setenv("SCRIPT_NAME", script_path.c_str(), 1);
	setenv("QUERY_STRING", query_string.c_str(), 1);
	
	// Create pipes for communication
	int input_pipe[2], output_pipe[2];
	if (pipe(input_pipe) < 0 || pipe(output_pipe) < 0) {
		throw std::runtime_error("Failed to create pipes");
	}

	pid_t pid = fork();
	if (pid < 0) {
		throw std::runtime_error("Fork failed");
	}

	if (pid == 0) {  // Child process
		close(input_pipe[1]);
		close(output_pipe[0]);
		
		// Redirect stdin/stdout
		dup2(input_pipe[0], STDIN_FILENO);
		dup2(output_pipe[1], STDOUT_FILENO);
		
		// Execute the script
		execl(script_path.c_str(), script_path.c_str(), nullptr);
		exit(1);
	}

	// Parent process
	close(input_pipe[0]);
	close(output_pipe[1]);

	// Write POST data if any
	if (!body.empty()) {
		write(input_pipe[1], body.c_str(), body.length());
	}
	close(input_pipe[1]);

	// Read response
	std::string output;
	char buffer[4096];
	ssize_t bytes_read;
	while ((bytes_read = read(output_pipe[0], buffer, sizeof(buffer))) > 0) {
		output.append(buffer, bytes_read);
	}
	close(output_pipe[0]);

	// Wait for child process
	int status;
	waitpid(pid, &status, 0);

	return output;
}
