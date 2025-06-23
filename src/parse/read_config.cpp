/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:40:10 by jgraf             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/06 12:33:52 by nmonzon          ###   ########.fr       */
=======
/*   Updated: 2025/06/23 09:13:02 by jgraf            ###   ########.fr       */
>>>>>>> master
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"
<<<<<<< HEAD
#include <filesystem>

static bool	is_separate_char(char chr)
{
	std::vector<char>	limiter = {';', ',', '(', ')', '[', ']', '{', '}'};

	for (char limit_char : limiter)
	{
		if (chr == limit_char)
			return (true);
	}
	return (false);
}

//	Loop through the line and push individual tokens into the vector.
static void	tokenize(const std::string &line, t_vecstr &tokens)
{
	const std::string	delims = " \t\n;,(){}[]";
	size_t				i = 0;
	size_t				len = line.size();

	while (i < len)
	{
		//skip delimiters that are NOT separate chars (whitespace, etc.)
		while (i < len && delims.find(line[i]) != std::string::npos && !is_separate_char(line[i]))
			i ++;

		//if at a separate char delimiter, push it as token and advance
		if (i < len && is_separate_char(line[i]))
		{
			tokens.push_back(std::string(1, line[i]));
			i ++;
			continue;
		}

		//now accumulate a token until next delimiter
		size_t	start = i;
		while (i < len && delims.find(line[i]) == std::string::npos)
			i ++;

		if (start < i)
			tokens.push_back(line.substr(start, i - start));
	}
	tokens.push_back("\n");
}
=======
>>>>>>> master

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
		throw WrongFileException();
	}

	//loop through input file
	while (std::getline(file, line))
		tokenize(line, tokens);

	//close file to avoid leaking
	file.close();
<<<<<<< HEAD
	return (output);
}
=======

	//assign token type
	assign_token_type(tokens);
	return(tokens);
}
>>>>>>> master
