/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:40:10 by jgraf             #+#    #+#             */
/*   Updated: 2025/05/19 14:20:44 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

//	Open and read the config file
void	read_config_file(std::string const &in_file)
{
	std::ifstream	file(in_file);
	std::string		line;

	//check successful openage
	if (!file.is_open() || file.fail() || std::filesystem::is_directory(in_file))
	{
		if (file.is_open())
			file.close();
		throw std::invalid_argument("ERROR: Failed to read from file.");
	}

	//read line
	while (std::getline(file, line))
		std::cout << line << std::endl;
	file.close();
}