#include "SocketManager.hpp"

// Constructor
SocketManager::SocketManager(std::vector<Server*> servers)
{
	for (Server *server : servers)
	{
		try
		{
			sockets.push_back(new Socket(server));
		}
		catch (const std::exception &e)
		{
			log(LOG_ERR, std::string(e.what()) + " Socket destroyed!");
			g_webserver->rmServer(server);
		}
	}
}

// Destructor
SocketManager::~SocketManager()
{
	for (Socket *socket : sockets)
		delete socket;
	sockets.clear();
}

// Initialize all the server sockets
void	SocketManager::initializeServerSockets()
{
	int		fd;
	pollfd	pfd;
	min_timeout = std::numeric_limits<size_t>::max();
	
	for (Socket *socket : sockets)
	{
		fd = socket->server_fd;
		fcntl(fd, F_SETFL, O_NONBLOCK);
		pfd = {fd, POLLIN | POLLOUT, 0};
		poll_fds.push_back(pfd);
		fd_to_socket[fd] = socket;
		min_timeout = std::min(min_timeout, socket->server->getTimeout());
	}
}

// Accept new connections
void	SocketManager::handleNewConnection(pollfd &pfd, time_t now)
{
	sockaddr_in	client_addr{};
	socklen_t	client_len = sizeof(client_addr);
	int			client_fd = accept(pfd.fd, (sockaddr*)&client_addr, &client_len);
	
	if (client_fd < 0)
	{
		log(LOG_ERR, "Accept failed: " + std::string(strerror(errno)));
		return;
	}
	
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	poll_fds.push_back({client_fd, POLLIN, 0});

	// Check fd_to_socket (catches weird linux-only bug with broken FDs upon first connection)
	if (fd_to_socket.count(pfd.fd) == 0 || fd_to_socket[pfd.fd] == nullptr)
	{
		log(LOG_ERR, "Error: fd_to_socket missing or null for fd " + std::to_string(pfd.fd));
		close(client_fd);
		return;
	}

	client_to_server[client_fd] = fd_to_socket[pfd.fd]->server;
	client_last_active[client_fd] = now;
	client_buffers[client_fd] = "";
}

//	Process request and respond from the server
bool	SocketManager::processRequest(int fd, std::string &request)
{
	size_t	headers_end = request.find("\r\n\r\n");
	
	if (headers_end != std::string::npos)
	{
		size_t	body_start = headers_end + 4;
		size_t	content_length = 0;
		size_t	cl_pos = request.find("Content-Length: ");

		if (cl_pos != std::string::npos)
		{
			size_t		cl_end = request.find("\r\n", cl_pos);
			std::string	cl_str = request.substr(cl_pos + 16, cl_end - (cl_pos + 16));
			content_length = std::stoul(cl_str);
		}
		if (content_length == 0 || request.size() >= body_start + content_length)
		{
			Server	*server = client_to_server[fd];
			server->respond(fd, request);
			return true;
		}
	}
	return false;
}

// Close the client connection and clean up all the maps and vectors
void	SocketManager::cleanupClient(int fd, size_t &i)
{
	close(fd);
	client_to_server.erase(fd);
	client_last_active.erase(fd);
	client_buffers.erase(fd);
	poll_fds.erase(poll_fds.begin() + i--);
}

// Recieves the request and forwards it to the parser in the revelant Server instance.
void	SocketManager::handleClientData(size_t &i, pollfd &pfd, time_t now)
{
	std::string		&request = client_buffers[pfd.fd];
	char			chunk[4096 * 4];
	ssize_t			bytes_read;
	bool			headers_complete = false;

	bytes_read = recv(pfd.fd, chunk, sizeof(chunk), 0);
	if (bytes_read > 0)
	{
		chunk[bytes_read] = '\0';
		request.append(chunk, bytes_read);

		if (!headers_complete && request.find("\r\n\r\n") != std::string::npos)
			headers_complete = true;
	}

	// Log the request
	log(LOG_INFO, "Request of size " + std::to_string(request.size()) + " bytes received from client (fd) "
	+ std::to_string(pfd.fd) + " in server "
	+ client_to_server[pfd.fd]->getName() + ":" + std::to_string(client_to_server[pfd.fd]->getPort()));

	if (bytes_read <= 0)
	{
		cleanupClient(pfd.fd, i);
		return;
	}

	if (processRequest(pfd.fd, request))
	{
		cleanupClient(pfd.fd, i);
		return;
	}

	// Update activity time for timeout check
	client_last_active[pfd.fd] = now;
}

void	SocketManager::handleErrors(size_t &i, pollfd &pfd)
{
	if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL))
		cleanupClient(pfd.fd, i);
}

void	SocketManager::checkTimeouts(time_t now)
{
	int	fd;

	for (size_t i = 0; i < poll_fds.size(); i++)
	{
		fd = poll_fds[i].fd;
		if (client_to_server.count(fd) && std::difftime(now, client_last_active[fd]) > client_to_server[fd]->getTimeout())
		{
			std::string response = client_to_server[fd]->createResponse(504, "", "");
			send(fd, response.c_str(), response.size(), 0);
			cleanupClient(fd, i);
		}
	}
}

void	SocketManager::run()
{
	initializeServerSockets(); // Set all the sockets up to read/write and be non-blocking

	while (true) // Runtime loop
	{
		if (poll(poll_fds.data(), poll_fds.size(), 100) < 0) // Listen to all sockets for incoming client activity
		{
			if (errno == EINTR)
				continue;
			log(LOG_ERR, "Poll error: " + std::string(strerror(errno)));
			break;
		}

		time_t	now = std::time(nullptr);

		// Quit if there are no valid servers running
		if (poll_fds.size() <= 0)
		{
			log(LOG_WARN, "No servers running. Quitting...");
			break;
		}

		// For every server socket poll-ed: establish a connection from the client and handle incoming request data
		for (size_t i = 0; i < poll_fds.size(); i++)
		{
			pollfd	&pfd = poll_fds[i];
			if ((pfd.revents & POLLIN) && fd_to_socket.count(pfd.fd))
			{
				handleNewConnection(pfd, now);
				continue;
			}
			if ((pfd.revents & POLLIN) && client_to_server.count(pfd.fd))
			{
				handleClientData(i, pfd, now);
				continue;
			}
			handleErrors(i, pfd); // Anything in poll() went wrong
		}
		checkTimeouts(now); // The client took too long to say anything
	}
}
