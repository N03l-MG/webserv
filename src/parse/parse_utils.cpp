/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:37:58 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/04 10:12:47 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool	brace_check(const t_vecstr tokens)
{
	int		counter = 0;
	size_t	i = 0;
	
	while (tokens[i] != "\0")
	{
		if (tokens[i] == "{")
			counter ++;
		if (tokens[i] == "}")
			counter --;
		i ++;
	}
	if (counter == 0)
		return (true);
	return (false);
}

bool	is_special_token(const std::string token)
{
	if (token == "(" || token == ")"
	||	token == "[" || token == "]"
	||	token == "{" || token == "}"
	||	token == "," || token == ";")
		return (true);
	return (false);
}

bool	line_has_semicolon(const t_vecstr tokens, size_t i)
{
	while (tokens[i] != "\0")
	{
		if (tokens[i] == ";")
			return (true);
		if (tokens[i] == "\n")
			return (false);
		i ++;
	}
	return (false);
}