/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getFile.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:57:01 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/24 18:30:25 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

enum e_action	setAction(std::string str)
{
	std::string	word = str.substr(0, str.find(" "));
	if (word == "GET")
		return GET;
	else if (word == "POST")
		return POST;
	else if (word == "DELETE")
		return DELETE;
	return INVALID;
}

std::string getFile(std::string fileAdr)
{
	std::ifstream file(fileAdr);
	std::string content;

	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		content = buffer.str();
		file.close();
	}

	return content;
}
