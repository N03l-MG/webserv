/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:16 by nmonzon           #+#    #+#             */
/*   Updated: 2025/07/04 10:45:25 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		log(LOG_ERR, "Too many arguments! (Expected 1 at most)");
		return (1);
	}

	try
	{
		WebServ *webserver = new WebServ;
		g_webserver = webserver;			//set the global pointer
		signal(SIGINT, signalHandler);		//register the signal handler

		if (ac == 2)
			webserver->setTokens(read_config_file(av[1]));
		else
			webserver->setTokens(read_config_file("./config/default.conf"));
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
