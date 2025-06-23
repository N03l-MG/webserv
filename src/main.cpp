/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:16 by nmonzon           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/06 12:33:23 by nmonzon          ###   ########.fr       */
=======
/*   Updated: 2025/06/23 12:23:13 by jgraf            ###   ########.fr       */
>>>>>>> master
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"
#include "WebServ.hpp"
<<<<<<< HEAD
#include <csignal>

WebServ *g_webserver = nullptr; // Global pointer to the webserver

void signalHandler(int signum) {
	if (signum == SIGINT) {
		std::cout << "Interrupt signal (" << signum << ") received.\n";
		if (g_webserver) {
			g_webserver->shutdown();
=======

WebServ	*g_webserver = nullptr; // Global pointer to the webserver

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		log(LOG_WARNING, "Interrupt signal (" + std::to_string(signum) + ") received.");
		if (g_webserver)
		{
			g_webserver->shutdown();
			g_webserver->~WebServ();
>>>>>>> master
		}
		exit(signum);
	}
}

<<<<<<< HEAD
int main(int ac, char **av) {
=======
int main(int ac, char **av)
{
>>>>>>> master
	if (ac != 2) {
		std::cerr << "Invalid number of arguments! (Expected 1)" << std::endl;
		return (1);
	}

<<<<<<< HEAD
	try {
=======
	try
	{
>>>>>>> master
		WebServ webserver;
		g_webserver = &webserver; // Set the global pointer
		signal(SIGINT, signalHandler); // Register the signal handler

<<<<<<< HEAD
		t_vecstr tokens = read_config_file(av[1]);
		webserver.start(tokens);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		if (g_webserver) {
			g_webserver->shutdown();
		}
		return (1);
	}

	if (g_webserver) {
		g_webserver->shutdown();
=======
		webserver.setTokens(read_config_file(av[1]));
		webserver.start();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		if (g_webserver)
			g_webserver->shutdown();
		return (1);
>>>>>>> master
	}

	if (g_webserver)
		g_webserver->shutdown();
	return (0);
}
