/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:57:45 by nmonzon           #+#    #+#             */
/*   Updated: 2025/07/01 12:29:05 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

void	log(t_logtype type, std::string msg)
{
	switch (type)
	{
		case LOG_INFO:
			std::cout << "\033[35m[INFO]\t\t"; break;
		case LOG_WARN:
			std::cout << "\033[33m[WARNING]\t"; break;
		case LOG_ERR:
			std::cout << "\033[31m[ERROR]\t"; break;
	}
	std::cout << msg << "\033[0m" << std::endl;
}