/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:42:33 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/14 13:50:37 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../include/webserv.hpp"

void processLine(std::string &line, std::string &directive)
{
	if (line.find("//") != std::string::npos)
		line = line.substr(0, line.find("//"));
	if (line.size() > 0)
		directive += line + "\n";
}

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
				processLine(line, serverDirective);
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
			processLine(line, serverDirective);
	}
	return servers;
}

std::vector<std::string> splitLocationDirectives(std::string serverDirectives)
{
	std::vector<std::string> locations;
	std::istringstream iss(serverDirectives);
	std::string line;
	std::string locationDirective;
	bool inLocation = false;

	locationDirective = line;
	while (std::getline(iss, line))
	{
		size_t openBrace = line.find("{");
		size_t closeBrace = line.find("}");

		if (openBrace != std::string::npos)
		{
			inLocation = true;
			if (line.find("location") != std::string::npos)
				line = line.substr(line.find("location") + 8);
			line = line.substr(0, line.find("{"));
			processLine(line, locationDirective);
		}
		else if (closeBrace != std::string::npos && inLocation)
		{
			inLocation = false;
			locations.push_back(locationDirective);
			locationDirective = "";
		}
		else if (inLocation)
			processLine(line, locationDirective);
	}
	return locations;
}

std::vector<Server> parseConfigFile(std::string file_name)
{
	std::string configFile = getFile(file_name);
	std::vector<std::string> directivesServers = splitServerDirectives(configFile);
	for (std::vector<std::string>::iterator it = directivesServers.begin(); it < directivesServers.end(); it++)
	{
		std::vector<std::string> directivesLocations = splitLocationDirectives(*it);
		std::cout << *it << std::endl;
		std::cout << "___________________________________________\nLocations:" << std::endl;
		for (std::vector<std::string>::iterator it2 = directivesLocations.begin(); it2 < directivesLocations.end(); it2++)
			std::cout << *it2 << std::endl << "______________________________" << std::endl;
		std::cout << "_______________" << std::endl;
	}
	return std::vector<Server>();
}
