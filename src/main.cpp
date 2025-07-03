/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:16 by nmonzon           #+#    #+#             */
/*   Updated: 2025/07/03 15:38:09 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"
#include "WebServ.hpp"

WebServ	*g_webserver = nullptr; // Global pointer to the webserver

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		log(LOG_WARN, "Interrupt signal (" + std::to_string(signum) + ") received.");
		if (g_webserver)
		{
			g_webserver->shutdown();
			g_webserver->~WebServ();
		}
		exit(signum);
	}
}


int main(int ac, char **av)
{
	if (ac > 2) {
		log(LOG_ERR, "Too many arguments!");
		return (1);
	}

	try
	{
		WebServ webserver;
		g_webserver = &webserver; // Set the global pointer
		signal(SIGINT, signalHandler); // Register the signal handler

		if (ac == 2)
			webserver.setTokens(read_config_file(av[1]));
		else
			webserver.setTokens(read_config_file("./config/default.conf"));
		webserver.start();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		if (g_webserver)
			g_webserver->shutdown();
		return (1);
	}

	if (g_webserver)
		g_webserver->shutdown();
	return (0);
}
