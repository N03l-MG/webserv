/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:35:06 by jgraf             #+#    #+#             */
/*   Updated: 2025/06/25 14:51:13 by nmonzon          ###   ########.fr       */
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
			return ("ERROR!\tFailed to open or read file! Default config may be missing or corrupt.");
		}
};

class ParseException : public std::exception
{
	public:
		const char	*what() const throw()
		{
			return ("ERROR!\tConfiguration file is invalid!");
		}
};
