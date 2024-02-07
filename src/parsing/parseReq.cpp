/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseReq.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 20:21:43 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/07 15:52:49 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

Request parseReq(std::string passedReq)
{
	std::istringstream iss(passedReq);
	std::string line;
	int count = 0;
	Request req;

	while (std::getline(iss, line, '\n'))
	{
		std::istringstream iss2(line);
		std::string word;
		while (std::getline(iss2, word, ' '))
		{
			switch (count)
			{
			case 0:
				req.method = word;
				break;
			case 1:
				req.uri = word;
				break;
			case 2:
				req.version = word;
				break;
			}
			count++;
		}
	}
	return req;
}
