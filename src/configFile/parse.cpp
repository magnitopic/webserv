/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:42:33 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/08 16:59:52 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

std::vector<std::string> splitServerDirectives(std::string configFiles)
{
	std::vector<std::string> serverDirectives;
	std::string delimiter = "server";
	size_t pos = 0;
	std::string token;

	while ((pos = configFiles.find(delimiter)) != std::string::npos)
	{
		token = configFiles.substr(0, pos);
		serverDirectives.push_back(token);
		configFiles.erase(0, pos + delimiter.length());
	}
	return serverDirectives;
}

int parseConfigFile(std::string file_name)
{
	std::string configFile = getFile(file_name);
	std::vector<std::string> servers = splitServerDirectives(configFile);
	for (std::vector<std::string>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		// std::cout << *it << std::endl;
		// std::cout << "_______________" << std::endl;
	}
	return 0;
}
