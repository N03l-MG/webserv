/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:57:45 by nmonzon           #+#    #+#             */
/*   Updated: 2025/06/23 10:23:44 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

void	log(t_logtype type, std::string msg)
{
	switch (type)
	{
		case LOG_LOG:
			std::cout << "\033[35mLOG:\t";
			break;
		case LOG_WARNING:
			std::cout << "\033[33mWARNING:\t";
			break;
		case LOG_ERROR:
			std::cout << "\033[31mERROR:\t";
			break;
	}
	std::cout << msg << "\033[0m" << std::endl;
}