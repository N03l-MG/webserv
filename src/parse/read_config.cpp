/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:40:10 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/06 12:33:52 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"
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

//	Read individual lines and call the tokenisation function on each line.
t_vecstr	read_config_file(std::string const &in_file)
{
	t_vecstr		output;
	std::ifstream	file(in_file);
	std::string		line;

	//safely open file
	if (!file.is_open() || file.fail() || std::filesystem::is_directory(in_file))
	{
		if (file.is_open())
			file.close();
		throw WrongFileException();
	}

	//loop through input file
	while (std::getline(file, line))
		tokenize(line, output);

	//close file to avoid leaking
	file.close();
	return (output);
}
