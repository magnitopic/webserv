/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseReq.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 20:21:43 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/07 15:54:41 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/webserv.hpp"

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
				req.setMethod(word);
				break;
			case 1:
				req.setUri(word);
				break;
			case 2:
				req.setVersion(word);
				break;
			}
			count++;
		}
	}
	return req;
}

int parsedContentLength(std::string str)
{
	size_t pos = str.find("Content-Length:") + 15;
	if (pos < 15 || pos > str.length())
		return -1;
	while (isspace(str[pos]))
		pos++;
	std::string num;
	while (isdigit(str[pos]))
	{
		num.push_back(str[pos]);
		pos++;
	}
	return atoi(num.c_str());
}

int greatExpectations(std::string str)
{
	size_t pos = str.find("Expect: 100-continue") + 20;
	if (pos < 20 || pos > str.length())
		return 0;
	return 1;
}
