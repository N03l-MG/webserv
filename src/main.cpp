/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:28:16 by nmonzon           #+#    #+#             */
/*   Updated: 2025/05/22 10:00:24 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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
		std::vector<std::string>	tokens;
		read_config_file(av[1], tokens);
		for (size_t i = 0; i < tokens.size(); i++)
			std::cout << tokens[i] << std::endl;
	}
	catch (std::invalid_argument &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}