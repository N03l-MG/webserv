/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:16 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/23 09:34:39 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"
#include "WebServ.hpp"

WebServ	*g_webserver = nullptr; // Global pointer to the webserver

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		std::cout << "Interrupt signal (" << signum << ") received.\n";
		if (g_webserver)
		{
			g_webserver->shutdown();
			g_webserver->~WebServ();	//probably doesn't trigger automatically because it's global
		}
		exit(signum);
	}
}

int main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Invalid number of arguments! (Expected 1)" << std::endl;
		return (1);
	}

	try
	{
		WebServ webserver;
		g_webserver = &webserver; // Set the global pointer
		signal(SIGINT, signalHandler); // Register the signal handler

		webserver.setTokens(read_config_file(av[1]));
		webserver.start();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		if (g_webserver)
		{
			g_webserver->shutdown();
			g_webserver->~WebServ();
		}
		return (1);
	}

	if (g_webserver)
	{
		g_webserver->shutdown();
		g_webserver->~WebServ();
	}
	return (0);
}
