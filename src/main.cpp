#include "include.hpp"
#include "WebServ.hpp"

WebServ	*g_webserver = nullptr;	//global pointer to the webserver

void	signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		log(LOG_WARN, "Interrupt signal (" + std::to_string(signum) + ") received.");
		if (g_webserver)
		{
			g_webserver->shutdown();
			delete g_webserver;
		}
		exit(signum);
	}
}


int	main(int ac, char **av)
{
	if (ac > 2)
	{
		log(LOG_ERR, "Too many arguments!");
		return (1);
	}

	try
	{
		WebServ	*webserver = new WebServ;
		g_webserver = webserver;			//set the global pointer
		signal(SIGINT, signalHandler);		//register the signal handler

		if (ac == 2)
			webserver->setTokens(read_config_file(ac, av[1]));
		else
			webserver->setTokens(read_config_file(ac, "./config/default.conf"));
		webserver->start();
	}
	catch (std::exception &e)
	{
		log(LOG_ERR, e.what());
		if (g_webserver)
			g_webserver->shutdown();
		delete g_webserver;
		g_webserver = nullptr;
		return (1);
	}

	if (g_webserver)
		g_webserver->shutdown();
	delete g_webserver;
	g_webserver = nullptr;
	return (0);
}
