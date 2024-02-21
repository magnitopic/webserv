/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitDirectives.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:39:08 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/21 14:55:45 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

static void processLine(std::string &line, std::string &directive)
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
			else
			{
				if (line.find("server") == std::string::npos)
					raiseError("Found invalid directive outside of server block");
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
			processLine(line, serverDirective);
	}
	if (servers.size() == 0)
		raiseError("No server directives found in the config fileeeee");
	return servers;
}

/**
 * Finds the location directives inside a server directive and stores each one
 * along with it's correspondig directives an a std::vector<std::string>
 */
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
