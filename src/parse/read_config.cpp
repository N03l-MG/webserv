/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:40:10 by jgraf             #+#    #+#             */
/*   Updated: 2025/07/04 13:29:07 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"
#include "WebServ.hpp"
#include <filesystem>

static bool	is_comment(const std::string line)
{
	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] == '#')
			return (true);
		if (line[i] != ' ' && line[i] != '\t')
			return (false);
	}
	return (false);
}

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
		throw	std::invalid_argument("Failed to open or read file! Default config may be missing or corrupt.");
	}

	//loop through input file
	while (std::getline(file, line))
	{
		if (!is_comment(line))
			tokenize(line, tokens);
	}

	//close file to avoid leaking
	file.close();

	//assign token type
	assign_token_type(tokens);
	return(tokens);
}