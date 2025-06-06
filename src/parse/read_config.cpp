/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:40:10 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 10:26:31 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

//	Read individual lines and call the tokenisation function on each line.
std::vector<t_tokens>	read_config_file(std::string const &in_file)
{
	std::vector<t_tokens>	tokens;
	std::ifstream			file(in_file);
	std::string				line;

	//safely open file
	if (!file.is_open() || file.fail() || std::filesystem::is_directory(in_file))
	{
		if (file.is_open())
			file.close();
		throw WrongFileException();
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