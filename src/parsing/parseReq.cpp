/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseReq.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 20:21:43 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/02 18:31:22 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../include/webserv.hpp"

Request parseReq(std::string req, Socket &socket)
{
	std::istringstream iss(req);
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
				req.
				break;
			
			default:
				break;
			}
			count++;
		}
			
	}
}
