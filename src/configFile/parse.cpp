/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:42:33 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/14 12:55:57 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../include/webserv.hpp"

/**
 * Finds the server directives in the file and returns a vector of strings.
 * Where each string is a server directive.
 */
std::vector<std::string> splitServerDirectives(std::string configFile)
{
	std::vector<std::string> servers;
	std::istringstream iss(configFile);
	std::string line;
	std::string serverDirective;
	int openBraces = 0;

	serverDirective = line;
	while (std::getline(iss, line))
	{
		size_t openBrace = line.find("{");
		size_t closeBrace = line.find("}");

		if (openBrace != std::string::npos)
		{
			if (openBraces >= 1)
			{
				if (line.find("//") != std::string::npos)
				line = line.substr(0, line.find("//"));
				if (line.size() > 0)
					serverDirective += line + "\n";
			}
			openBraces++;
		}
		else if (closeBrace != std::string::npos && openBraces > 0)
		{
			openBraces--;
			if (openBraces == 0)
			{
				servers.push_back(serverDirective);
				serverDirective = "";
			}
			else
				serverDirective += line + "\n";
		}
		else if (openBraces > 0)
		{
			// if there is a comment (//) at the end of the line remove it
			if (line.find("//") != std::string::npos)
				line = line.substr(0, line.find("//"));
			if (line.size() > 0)
				serverDirective += line + "\n";
		}
	}
	return servers;
}

std::vector<Server> parseConfigFile(std::string file_name)
{
	std::string configFile = getFile(file_name);
	std::vector<std::string> servers = splitServerDirectives(configFile);
	for (std::vector<std::string>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::cout << *it << std::endl;
		std::cout << "_______________" << std::endl;
	}
	return std::vector<Server>();
}
