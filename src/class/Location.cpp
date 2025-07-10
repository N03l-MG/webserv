#include "Location.hpp"

//	Constructor
Location::Location(Server *parent_server)
{
	log(LOG_INFO, "Location created!");
	this->path = "/";
	this->index = "";
	this->max_body = parent_server->getMaxBody();
}


//	Destructor
Location::~Location()
{
	log(LOG_INFO, "Location destroyed!");
}


//	Setters
void	Location::setPath(std::string path)			{ this->path = path; }
void	Location::setIndex(std::string index)		{ this->index = index; }
void	Location::setMaxBody(size_t max_body)		{ this->max_body = max_body; }
void	Location::addMethod(std::string method)		{ this->allow_methods.push_back(method); }
void	Location::addCgi(std::string path)			{ this->cgi_scripts.push_back(path); }
void	Location::addReturn(std::string _return)	{ this->_return.push_back(_return); }


//	Getters
std::string	Location::getPath()		{ return (this->path); }
std::string	Location::getIndex()	{ return (this->index); }
size_t		Location::getMaxBody()	{ return (this->max_body); }
t_vecstr	Location::getMethod()	{ return (this->allow_methods); }
t_vecstr	Location::getCgi()		{ return (this->cgi_scripts); }
t_vecstr	Location::getReturn()	{ return (this->_return); }
std::string	Location::getMethod(size_t index)
{
	if (index < allow_methods.size())
		return (allow_methods[index]);
	return ("");
}
std::string	Location::getCgi(size_t index)
{
	if (index < cgi_scripts.size())
		return (cgi_scripts[index]);
	return ("");
}
std::string	Location::getReturn(size_t index)
{
	if (index < _return.size())
		return (_return[index]);
	return ("");
}


//	Config
bool	Location::checkMethod(std::string method)
{
	if (method == "GET" || method == "POST" || method == "DELETE")
		return true;
	return false;
}

void	Location::configure(t_vectok &tokens, size_t &i)
{
	//set path
	while (tokens[i].type != TOK_OPEN_BRACE)
	{
		if (tokens[i].type == TOK_VALUE)
			setPath(tokens[i++].token);
	}
	
	//configure
	while (i + 1 < tokens.size() && tokens[++i].type != TOK_CLOSE_BRACE)
	{
		if (tokens[i].type == TOK_KEY)
		{
			std::string key = tokens[i].token;
			while (i + 1 < tokens.size() && tokens[++i].type != TOK_SEMICOLON && tokens[i].type != TOK_CLOSE_BRACE)
			{
				std::string value = tokens[i].token;
				if (key == "index")
					setIndex(value);
				else if (key == "max_body")
					setMaxBody(std::stoi(value));
				else if (key == "methods" && checkMethod(value))
					addMethod(value);
				else if (key == "cgi")
					addCgi(value);
				else if (key == "return")
					addReturn(value);
			}
		}
	}

	//print debug statement
	//print_status();
}


//	Debug
void	Location::print_status()
{
	std::cout << "\n\t--- LOCATION CONFIG ---" << std::endl;
	std::cout << "Path:\t\t" << getPath() << "\n"
			<< "Index:\t\t" << getIndex() << "\n"
			<< "Body Size\t" << getMaxBody() << std::endl;
	
	
	for (size_t i = 0; i < allow_methods.size(); i++)
		std::cout << "Methods:\t" << getMethod(i) << std::endl;
	for (size_t i = 0; i < cgi_scripts.size(); i++)
		std::cout << "CGI Path:\t" << getCgi(i) << std::endl;
	for (size_t i = 0; i < _return.size(); i++)
		std::cout << "Return:\t\t" << getReturn(i) << std::endl;
}
