/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:16 by nmonzon           #+#    #+#             */
/*   Updated: 2025/05/28 11:41:49 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "WebServer.hpp"

int main(int ac, char **av)
{
	//input check
	if (ac != 2)
	{
		std::cerr << "Invalid number of arguments! (Expected 1)" << std::endl;
		return (1);
	}

	//try to read input file
	try
	{
		WebServer	webserver;
		t_vecstr	tokens = read_config_file(av[1]);
		webserver.start(tokens);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}