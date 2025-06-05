/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:16 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/05 16:08:09 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "WebServer.hpp"
#include <csignal>

WebServer *g_webserver = nullptr; // Global pointer to the webserver

void signalHandler(int signum) {
	if (signum == SIGINT) {
		std::cout << "Interrupt signal (" << signum << ") received.\n";
		if (g_webserver) {
			g_webserver->shutdown();
		}
		exit(signum);
	}
}

int main(int ac, char **av) {
	if (ac != 2) {
		std::cerr << "Invalid number of arguments! (Expected 1)" << std::endl;
		return (1);
	}

	try {
		WebServer webserver;
		g_webserver = &webserver; // Set the global pointer
		signal(SIGINT, signalHandler); // Register the signal handler

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
	}
	return (0);
}
