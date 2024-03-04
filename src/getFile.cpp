/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getFile.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:57:01 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/04 07:57:49 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/webserv.hpp"

std::string getFile(const std::string &fileAdr)
{
	std::ifstream file(fileAdr, std::ios::in | std::ios::binary);
	std::string content = "";

	if (file.is_open())
	{
		std::stringstream buffer;
		buffer << file.rdbuf();
		content = buffer.str();
		file.close();
	}
	return content;
}
