/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:16 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/06 12:54:52 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"
#include "WebServ.hpp"

int main(int ac, char **av)
{
	//input check
	if (ac != 2)
	{
		std::cerr << "Invalid number of arguments! (Expected 1)" << std::endl;
		return (1);
	}

	//try to read input file and execute program
	try
	{
		WebServ	webserver;
		webserver.setTokens(read_config_file(av[1]));
		webserver.start();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}