/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:35:06 by jgraf             #+#    #+#             */
/*   Updated: 2025/07/03 15:42:18 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Guard
#pragma once

//	Include
#include <exception>

//	Exception
class WrongFileException : public std::exception
{
	public:
		const char	*what()	const throw()
		{
			return ("ERROR!\tFailed to open or read configuration file.");
		}
};

class MissingDefaultException : public std::exception
{
	public:
		const char *what() const throw()
		{
			return ("ERROR!\tDefault configuration missing or broken. Please try passing a specific file.");
		}
};

class ParseException : public std::exception
{
	public:
		const char	*what() const throw()
		{
			return ("ERROR!\tConfiguration file is invalid.");
		}
};
