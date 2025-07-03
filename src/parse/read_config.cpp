/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:40:10 by jgraf             #+#    #+#             */
/*   Updated: 2025/07/03 15:42:47 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"
#include <filesystem>

//	Read individual lines and call the tokenisation function on each line.
t_vectok	read_config_file(std::string const &in_file)
{
	t_vectok	tokens;
	std::ifstream			file(in_file);
	std::string				line;

	//safely open file
	if (!file.is_open() || file.fail() || std::filesystem::is_directory(in_file))
	{
		if (file.is_open())
			file.close();
		if (in_file == "./config/default.conf")
			throw	MissingDefaultException();
		throw	WrongFileException();
	}

	//loop through input file
	while (std::getline(file, line))
		tokenize(line, tokens);

	//close file to avoid leaking
	file.close();

	//assign token type
	assign_token_type(tokens);
	return(tokens);
}