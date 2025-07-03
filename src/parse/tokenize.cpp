/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:30:28 by jgraf             #+#    #+#             */
/*   Updated: 2025/07/03 10:24:15 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"
#include <algorithm>

static bool	is_separate_char(char chr)
{
	std::vector<char>	limiter = {';', '{', '}'};

	for (char limit_char : limiter)
	{
		if (chr == limit_char)
			return (true);
	}
	return (false);
}

//	Loop through the line and push individual tokens into the vector.
void	tokenize(const std::string &line, t_vectok &tokens)
{
	const std::string	delims = " \t\n;{}#";
	size_t				i = 0;
	size_t				len = line.size();

	//read through line
	while (i < len)
	{
		//skip delimiters that are NOT separate chars (whitespace, etc.)
		while (i < len && delims.find(line[i]) != std::string::npos && !is_separate_char(line[i]))
			i ++;

		//if at a separate char delimiter, push it as token and advance
		if (i < len && is_separate_char(line[i]))
		{
			t_tokens	new_tok;
			new_tok.token = std::string(1, line[i++]);
			tokens.push_back(new_tok);
			continue;
		}

		//now accumulate a token until next delimiter
		size_t	start = i;
		while (i < len && delims.find(line[i]) == std::string::npos)
			i ++;
		if (start < i)
		{
			t_tokens	new_tok;
			new_tok.token = line.substr(start, i - start);
			tokens.push_back(new_tok);
		}
	}
}

//	Assign types to the individual tokens
void	assign_token_type(t_vectok &tokens)
{
	t_vecstr	directives = {"server", "location"};
	t_vecstr	keys = {"listen", "host", "root", "index", "path", "return", "timeout", "max_body", "methods", "cgi", "error_page"};

	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (std::find(directives.begin(), directives.end(), tokens[i].token) != directives.end())
			tokens[i].type = TOK_DIRECTIVE;
		else if (std::find(keys.begin(), keys.end(), tokens[i].token) != keys.end())
			tokens[i].type = TOK_KEY;
		else if (tokens[i].token == "{")
			tokens[i].type = TOK_OPEN_BRACE;
		else if (tokens[i].token == "}")
			tokens[i].type = TOK_CLOSE_BRACE;
		else if (tokens[i].token == ";")
			tokens[i].type = TOK_SEMICOLON;
		else if (tokens[i].token == "\0")
			tokens[i].type = TOK_END;
		else
			tokens[i].type = TOK_VALUE;
	}
}