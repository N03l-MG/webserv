/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:21:23 by jgraf             #+#    #+#             */
/*   Updated: 2025/07/07 15:08:54 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/wait.h>

//	Constructor
Server::Server()
{
	log(LOG_INFO, "Server created!");
	this->port = 8080;
	this->host = "127.0.0.1";
	this->name = "localhost";
	this->root = "www/";
	this->index = "index.html";
	this->timeout = 15;
	this->max_body = 5000000;
	this->last_active = std::time(NULL);

	//mime types
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

	//status codes
	statusCodes[200] = "OK";
	statusCodes[204] = "No Content";
	statusCodes[400] = "Bad Request";
	statusCodes[403] = "Forbidden";
	statusCodes[404] = "Not Found";
	statusCodes[405] = "Method Not Allowed";
	statusCodes[413] = "Payload Too Large";
	statusCodes[500] = "Internal Server Error";
}

//	Destructor
Server::~Server()
{
	//delete all locations to avoid leaking
	for (Location *loc : locations)
		delete loc;
	locations.clear();
	error_page.clear();
	log(LOG_INFO, "Server destroyed!");
}

//	Setters
void	Server::setPort(int port)							{ this->port = port; }
void	Server::setHost(std::string host)					{ this->host = host; }
void	Server::setName(std::string name)					{ this->name = name; }
void	Server::setRoot(std::string root)					{ this->root = root; }
void	Server::setIndex(std::string index)					{ this->index = index; }
void	Server::setTimeout(size_t timeout)					{ this->timeout = timeout; }
void	Server::setMaxBody(size_t max_body)					{ this->max_body = max_body; }
void	Server::addErrorpage(size_t code, std::string page)	{ this->error_page[code] = page; }
int		Server::addLocation(Location *new_location)
{
	if (!new_location)
		return (1);
	locations.push_back(new_location);
	return (0);
}

//	Getters
int			Server::getPort()							{ return (this->port); }
std::string	Server::getHost()							{ return (this->host); }
std::string	Server::getName()							{ return (this->name); }
std::string	Server::getRoot()							{ return (this->root); }
std::string	Server::getIndex()							{ return (this->index); }
size_t		Server::getTimeout()						{ return (this->timeout); }
size_t		Server::getMaxBody()						{ return (this->max_body); }
std::vector<Location*>	Server::getLocation()			{ return (this->locations); }
std::map<size_t, std::string>	Server::getErrorpage()	{ return (this->error_page); }
std::string	&Server::getErrorpage(size_t code)			{ return (this->error_page[code]); }
Location	*Server::getLocation(size_t index)
{
	if (index < locations.size())
		return (locations[index]);
	return (NULL);
}


















//	Configuration
bool	Server::braceCheck(t_vectok tokens)
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
	if (!braceCheck(tokens))
		throw	std::runtime_error("Configuration file is invalid!");

	//set server_name
	while (tokens[i].type != TOK_OPEN_BRACE)
	{
		if (tokens[i].type == TOK_VALUE)
			setName(tokens[i++].token);
	}

	//configure
	while (tokens[++i].type != TOK_CLOSE_BRACE)
	{
		if (tokens[i].type == TOK_KEY)
		{
			key = tokens[i++].token;
			value = tokens[i].token;
			if (tokens[i].type != TOK_VALUE)
				throw	std::runtime_error("Configuration file is invalid!");

			if (key == "host")
				setHost(value);
			else if (key == "listen")
				setPort(std::stoi(value));
			else if (key == "root")
				setRoot(value);
			else if (key == "index")
				setIndex(value);
			else if (key == "timeout")
				setTimeout(std::stoi(value));
			else if (key == "max_body")
				setMaxBody(std::stoi(value));
			else if (key == "error_page")
			{
				if (tokens[++i].type != TOK_VALUE)
					throw	std::runtime_error("Configuration file is invalid!");
				std::string page = tokens[i].token;
				addErrorpage(std::stoi(value), page);
			}
		}
		else if (tokens[i].type == TOK_DIRECTIVE && tokens[i].token == "location")
		{
			Location	*new_location = new Location(this);
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
			<< "Index:\t\t" << getIndex() << "\n"
			<< "Timeout:\t" << getTimeout() << "\n"
			<< "Max Body:\t" << getMaxBody() << std::endl;
	
	for (size_t i = 0; i < locations.size(); i++)
		std::cout << "Locations:\t" << getLocation(i)->getPath() << std::endl;
	for (std::map<size_t, std::string>::iterator it = error_page.begin(); it != error_page.end(); it++)
		std::cout << "Error Code:\t" << it->first  << "\t-> Page:\t" << it->second << std::endl;
}

















//	Get Mime type
std::string	Server::getMimeType(const std::string &filepath)
{
	size_t	dot_pos = filepath.find_last_of(".");

	if (dot_pos != std::string::npos)
	{
		auto	it = mimeTypes.find(filepath.substr(dot_pos));
		if (it != mimeTypes.end())
			return (it->second);
	}
	return ("application/octet-stream");
}

//	Check if request has cgi permissions
bool	Server::isCgi(const HttpRequest &request)
{
	if (!request.location)
		return false;
	const std::vector<std::string>& cgiSuffixes = request.location->getCgi();
	const std::string& path = request.path;

	//strip query string
	size_t query_pos = path.find('?');
	std::string clean_path = (query_pos != std::string::npos) ? path.substr(0, query_pos) : path;

	//find file extension
	size_t dot_pos = clean_path.find_last_of('.');
	if (dot_pos == std::string::npos)
		return false;

	std::string ext = clean_path.substr(dot_pos);
	//match against configured suffixes
	for (const std::string& suffix : cgiSuffixes)
		if (ext == suffix)
			return true;
	return false;
}

//	Check if requested method is allowed
bool	Server::checkMethods(const HttpRequest &request)
{
	t_vecstr allowed_methods = request.location->getMethod();
	for (const std::string &method : allowed_methods)
		if (method == request.method)
			return true;
	return false;
}

















//	Request parsing utils
//	Replace the alias path with the original location path
std::string	Server::normalizePath(const std::string &path)
{
	size_t		pos;
	std::string	normalized = path;

	while ((pos = normalized.find("//")) != std::string::npos)
		normalized.erase(pos, 1);
	if (normalized.length() > 1 && normalized.back() == '/')
		normalized.pop_back();
	return (normalized);
}

void	Server::CheckAlias(std::string &path)
{
	//separate the path and arguments if a ? is found
	size_t		args_pos = path.find('?');
	std::string	base_path = normalizePath((args_pos != std::string::npos) ? path.substr(0, args_pos) : path);
	std::string	arguments = (args_pos != std::string::npos) ? path.substr(args_pos) : "";

	//loop through all locations and their aliases, replace the alias with the location path
	for (Location *location : locations)
		for (const std::string &alias : location->getReturn())
		{
			std::string	normalized_alias = normalizePath(alias);
			if (base_path.find(normalized_alias) == 0)
			{
				path = location->getPath() + base_path.substr(normalized_alias.length()) + arguments;
				return;
			}
		}
}

//	Decode percent encoded paths and arguments
void	Server::percentDecode(std::string &body)
{
	if (body.empty())
		return;
	
	size_t								pos = 0;
	std::map<std::string, std::string>	repl;

	repl["%20"] = " ";
	repl["%3A"] = ":";
	repl["%2F"] = "/";
	repl["%3F"] = "?";
	repl["%23"] = "#";
	repl["%5B"] = "[";
	repl["%5D"] = "]";
	repl["%40"] = "@";
	repl["%21"] = "!";
	repl["%22"] = "\"";
	repl["%24"] = "$";
	repl["%5E"] = "^";
	repl["%26"] = "&";
	repl["%27"] = "'";
	repl["%28"] = "(";
	repl["%29"] = ")";
	repl["%2A"] = "*";
	repl["%2B"] = "+";
	repl["%2C"] = ",";
	repl["%3B"] = ";";
	repl["%3D"] = "=";
	repl["%25"] = "%";

	for (std::map<std::string, std::string>::iterator it = repl.begin(); it != repl.end(); it++)
	{
		pos = 0;
		while ((pos = body.find(it->first, pos)) != std::string::npos)
		{
			body.replace(pos, it->first.length(), it->second);
			pos += it->second.length();
		}
	}
}
















//	Parse request
Server::HttpRequest	Server::parseRequest(const std::string &raw_request) 
{
	HttpRequest			request;
	std::istringstream	request_stream(raw_request);
	std::string			request_line;
	std::string			header_line;

	//parse request line (e.g. GET /path HTTP/1.1)
	std::getline(request_stream, request_line);
	std::istringstream	request_line_stream(request_line);
	request_line_stream >> request.method >> request.path >> request.version;

	std::string longest_match = "";
	Location* matching_location = nullptr;
	std::string request_path = request.path;

	if (request_path[0] != '/')
		request_path = "/" + request_path;
	if (request_path.back() != '/')
		request_path += '/';
	for (Location *location : locations)
	{
		std::string loc_path = location->getPath();
		if (loc_path[0] != '/')
			loc_path = "/" + loc_path;
		if (loc_path.back() != '/')
			loc_path += '/';
		
		//get the first directory of the request path
		size_t request_first_dir = request_path.find('/', 1);
		if (request_first_dir == std::string::npos)
			request_first_dir = request_path.length();
		std::string request_dir = request_path.substr(0, request_first_dir);
		
		//check if this location matches
		if (request_path.find(loc_path) == 0 ||									//full path match
			(loc_path == "/" && request_path != "/") ||							//root location as fallback
			(loc_path.substr(0, loc_path.length() - 1) == request_dir + "/"))	//directory match
		{
			//update if this is the longest match so far
			if (loc_path.length() > longest_match.length())
			{
				longest_match = loc_path;
				matching_location = location;
			}
		}
	}
	if (matching_location)
		request.location = matching_location;

	//read header from request
	while (std::getline(request_stream, header_line) && header_line != "\r")
	{
		size_t	colon_pos = header_line.find(':');
		if (colon_pos != std::string::npos)
		{
			std::string	key = header_line.substr(0, colon_pos);
			std::string	value = header_line.substr(colon_pos + 2);
			if (!value.empty() && value[value.length()-1] == '\r')
				value = value.substr(0, value.length()-1);
			request.headers[key] = value;
		}
	}

	//search for content-type header
	auto	content_type_it = request.headers.find("Content-Type");
	if (content_type_it != request.headers.end())
	{
		size_t	boundary_pos = content_type_it->second.find("boundary=");
		if (boundary_pos != std::string::npos)
			request.boundary = content_type_it->second.substr(boundary_pos + 9);
	}

	//get body of request
	size_t	header_end = raw_request.find("\r\n\r\n");
	if (header_end != std::string::npos)
		request.body = raw_request.substr(header_end + 4);
	CheckAlias(request.path);
	percentDecode(request.path);
	return request;
}
















//	Call the requested method and call their handler
void	Server::respond(int client_fd, const std::string &raw_request)
{
	HttpRequest	request = parseRequest(raw_request);
	log(LOG_INFO, "Method: " + request.method + " Version: " + request.version + " Location: " + request.path);

	//check max body size and return 413 in case of errors
	size_t max_body = request.location ? request.location->getMaxBody() : this->max_body;
	if (max_body > 0 && request.body.size() > max_body)
	{
		std::string	response = createResponse(413, "", "");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}

	if (isCgi(request))
		handleCgi(client_fd, request);
	else if (request.method == "POST")
		handlePost(client_fd, request);
	else if (request.method == "DELETE")
		handleDelete(client_fd, request);
	else
		handleGet(client_fd, request);
}

//	Create response using code
std::string	Server::createResponse(int status_code, const std::string &content_type, const std::string &body)
{
	std::string status_text = "Internal Server Error";
	std::string response_body = body;
	std::string response_content_type = content_type;
	if (statusCodes.find(status_code) != statusCodes.end())
		status_text = statusCodes[status_code];

	//check if an error page is configured for the status code
	if (error_page.find(status_code) != error_page.end())
	{
		std::string     error_page_path = root + error_page[status_code];
		std::ifstream   error_file(error_page_path, std::ios::binary | std::ios::ate);
		if (error_file.is_open())
		{
			std::streamsize     size = error_file.tellg();
			std::vector<char>   file_buffer(size);
			error_file.seekg(0, std::ios::beg);
			error_file.read(file_buffer.data(), size);
			error_file.close();
			response_body = std::string(file_buffer.data(), file_buffer.size());
			response_content_type = getMimeType(error_page_path);
		}
	}

	return ("HTTP/1.1 " + std::to_string(status_code) + " " + status_text + "\r\n"
			"Content-Type: " + response_content_type + "\r\n"
			"Content-Length: " + std::to_string(response_body.length()) + "\r\n"
			"Connection: close\r\n"
			"\r\n" +
			response_body);
}
















//	GET Method
void	Server::handleGet(int client_fd, const HttpRequest &request)
{
	std::string	path = request.path;
	std::string	response;
	std::string	content_type;
	std::string	filepath;

	//check if the path is empty or root, and replace with default index
	if (path.empty() || path == "/")
		path = "/" + this->index;
	filepath = this->root + path;

	//if directory replace with the indexed file if it exists
	if (std::filesystem::is_directory(filepath))
	{
		if (request.location && !request.location->getIndex().empty())
			filepath += "/" + request.location->getIndex();
		else
			filepath += "/" + this->index;
	}

	//open the file and send an error if not found
	std::ifstream file(filepath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		response = createResponse(404, "", "");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}
	else if (!checkMethods(request))
	{
		response = createResponse(405, "", "");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}

	//read file content
	std::streamsize		size = file.tellg();
	std::vector<char>	file_buffer(size);
	file.seekg(0, std::ios::beg);
	file.read(file_buffer.data(), size);
	file.close();

	//send the file content
	content_type = getMimeType(filepath);
	response = createResponse(200, content_type, std::string(file_buffer.data(), file_buffer.size()));
	send(client_fd, response.c_str(), response.size(), 0);
}
















//	POST Method
//	Get file information
std::pair<std::string, std::string>	Server::extractFileInfo(const HttpRequest &request)
{
	size_t		filename_pos;
	size_t		filename_end;
	std::string	filename;
	size_t		content_start;
	size_t		content_end;
	std::string	file_content;

	//kill if no bounday has been found
	if (request.boundary.empty())
		throw	std::runtime_error("Bad Request: No boundary found");

	//find filename
	filename_pos = request.body.find("filename=\"");
	if (filename_pos == std::string::npos)
		throw	std::runtime_error("Bad Request: No filename found");

	//get filename
	filename_pos += 10;
	filename_end = request.body.find("\"", filename_pos);
	filename = request.body.substr(filename_pos, filename_end - filename_pos);

	//find file content
	content_start = request.body.find("\r\n\r\n", filename_end) + 4;
	content_end = request.body.find("--" + request.boundary + "--") - 2;
	if (content_start == std::string::npos || content_end == std::string::npos)
		throw	std::runtime_error("Bad Request: Invalid file content");

	//get file content
	file_content = request.body.substr(content_start, content_end - content_start);
	return (std::make_pair(filename, file_content));
}

//	Save the file to the server
void	Server::saveFile(const std::string &filename, const std::string &file_content, int client_fd, const std::string &path)
{
	std::string	response;
	std::string	filepath;
	std::string	upload_dir = getRoot() + path;

	//upload failed due to missing and uninstatiable (<--- likely misspelled ¯\_(ツ)_/¯) upload directory
	if (!std::filesystem::exists(upload_dir) && !std::filesystem::create_directory(upload_dir))
	{
		response = createResponse(500, "", "");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}

	//failed to create file
	filepath = upload_dir + "/" + filename;
	std::ofstream	outfile(filepath, std::ios::binary);
	if (!outfile.is_open())
	{
		response = createResponse(500, "", "");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}

	//write and close to outfile
	outfile.write(file_content.c_str(), file_content.length());
	outfile.close();

	//create and send response
	response = createResponse(201, "text/html", "");
	send(client_fd, response.c_str(), response.size(), 0);
}

// Main POST function
void	Server::handlePost(int client_fd, const HttpRequest &request)
{
	std::string response;

	if (!checkMethods(request))
	{
		response = createResponse(405, "", "");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}

	try
	{
		std::string filename;
		std::string file_content;

		// Extract Content-Type from headers
		auto content_type_it = request.headers.find("Content-Type");
		std::string content_type = (content_type_it != request.headers.end()) ? content_type_it->second : "text/plain";

		// Check if it's multipart/form-data
		if (content_type.find("multipart/form-data") != std::string::npos)
			std::tie(filename, file_content) = extractFileInfo(request);
		else
		{
			// Handle raw/plain requests
			filename = "upload_" + std::to_string(std::time(nullptr)) + ".txt";
			file_content = request.body;
		}

		saveFile(filename, file_content, client_fd, request.path);
	}
	catch (const std::runtime_error &e)
	{
		response = createResponse(400, "", "");
		send(client_fd, response.c_str(), response.size(), 0);
	}
}

















//	DELETE Method
void	Server::handleDelete(int client_fd, const HttpRequest &request)
{
	std::string	response;
	std::string	actual_path;
	std::string	full_path;

	//check to make sure users can't delete the entire server
	if (!checkMethods(request))
	{
		response = createResponse(405, "", "");
		send(client_fd, response.c_str(), response.size(), 0);
		return;
	}

	//remove www/ from filepath if it exists
	actual_path = request.path;
	if (actual_path.substr(0, 4) == "www/")
		actual_path = actual_path.substr(4);
	
	//construct full path
	full_path = "www/" + actual_path;
	if (std::filesystem::exists(full_path))
	{
		if (std::remove(full_path.c_str()) == 0)
		{
			response = createResponse(204, "text/plain", "File deleted\r\n");
			send(client_fd, response.c_str(), response.size(), 0);
		}
		else
		{
			response = createResponse(403, "", "");
			send(client_fd, response.c_str(), response.size(), 0);
		}
	}
	else
	{
		response = createResponse(404,  "", "");
		send(client_fd, response.c_str(), response.size(), 0);
	}
}















//	CGI
void	Server::handleCgi(int client_fd, const HttpRequest &request)
{
	size_t		query_pos = request.path.find('?');
	std::string	response;
	std::string	output;

	//extract script path and query string
	std::string script_path = this->root + request.path.substr(0, query_pos);
	std::string query_string = (query_pos != std::string::npos) ? request.path.substr(query_pos + 1) : "";
	try
	{
		output = executeCgi(script_path, query_string, request.method, request.body);
		response = createResponse(200, "text/html", output);
		send(client_fd, response.c_str(), response.size(), 0);
	}
	catch (const std::exception &e)
	{
		if (std::string(e.what()) == "CGI script timeout")
			response = createResponse(504,  "", "");
		else
			response = createResponse(500,  "", "");
		send(client_fd, response.c_str(), response.size(), 0);
	}
}

std::string Server::executeCgi(const std::string &script_path, const std::string &query_string,
								const std::string &method, const std::string &body)
{
	// Prepare environment variables
	std::vector<std::string> env_vars;
	env_vars.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env_vars.push_back("REQUEST_METHOD=" + method);
	env_vars.push_back("SCRIPT_NAME=" + script_path);
	env_vars.push_back("QUERY_STRING=" + query_string);
	env_vars.push_back("SERVER_PROTOCOL=HTTP/1.1");

	if (method == "POST") {
		env_vars.push_back("CONTENT_LENGTH=" + std::to_string(body.size()));
		env_vars.push_back("CONTENT_TYPE=application/x-www-form-urlencoded");
	}

	// Convert env vector to char* array
	std::vector<char*> envp;
	for (size_t i = 0; i < env_vars.size(); ++i)
		envp.push_back(const_cast<char*>(env_vars[i].c_str()));
	envp.push_back(nullptr);

	// Setup pipes
	int input_pipe[2];
	int output_pipe[2];
	if (pipe(input_pipe) < 0 || pipe(output_pipe) < 0)
		throw	std::runtime_error("Failed to create pipes");

	pid_t pid = fork();
	if (pid < 0)
		throw	std::runtime_error("Fork failed");

	if (pid == 0) {
		// In child process
		dup2(input_pipe[0], STDIN_FILENO);
		dup2(output_pipe[1], STDOUT_FILENO);

		close(input_pipe[1]);
		close(output_pipe[0]);

		char *const argv[] = {const_cast<char*>(script_path.c_str()), nullptr};

		execve(script_path.c_str(), argv, envp.data());

		// If execve fails
		exit(1);
	}

	// In parent process
	close(input_pipe[0]);
	close(output_pipe[1]);

	if (method == "POST" && !body.empty())
	if (method == "POST" && !body.empty())
		write(input_pipe[1], body.c_str(), body.length());
	close(input_pipe[1]);

	std::string output;
	char buffer[4096];
	ssize_t bytes_read;

	// Set output_pipe non-blocking
	fcntl(output_pipe[0], F_SETFL, O_NONBLOCK);


	time_t start = std::time(NULL);
	while (true) {
		last_active = std::time(NULL);
		if (static_cast<size_t>(std::time(NULL) - start) > timeout) {
			// Kill child ( ͡° ͜ʖ ͡°)
			kill(pid, SIGKILL);
			waitpid(pid, nullptr, 0);
			throw std::runtime_error("CGI script timeout");
		}

		bytes_read = read(output_pipe[0], buffer, sizeof(buffer));
		if (bytes_read > 0)
		{
			output.append(buffer, bytes_read);
			last_active = std::time(NULL);
		}
		else if (bytes_read == 0)
			break; // EOF
	}
	close(output_pipe[0]);

	int status;
	waitpid(pid, &status, 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		throw std::runtime_error("CGI script failed");

	return output;
}

