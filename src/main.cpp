/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:16 by nmonzon           #+#    #+#             */
/*   Updated: 2025/05/19 14:20:11 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Invalid number of arguments! (Expected 1)" << std::endl;
		return (1);
	}

	try
	{
		read_config_file(av[1]);
	}
	catch (std::invalid_argument &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}