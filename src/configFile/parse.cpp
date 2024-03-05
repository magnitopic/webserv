/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:42:33 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/05 14:36:42 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

void parseConfigFile(std::string file_name, std::vector<Server> &serverList)
{
	std::string configFile = getFile(file_name);
	std::vector<std::string> directivesServers = splitServerDirectives(configFile);

	for (std::vector<std::string>::iterator it = directivesServers.begin(); it < directivesServers.end(); it++)
	{
		std::string aux1 = *it;
		serverList.push_back(Server(aux1));
		std::vector<std::string> directivesLocations = splitLocationDirectives(aux1);
		std::vector<Location> locationsList;
		for (std::vector<std::string>::iterator it2 = directivesLocations.begin(); it2 != directivesLocations.end(); it2++)
		{
			locationsList.push_back(Location(*it2));
			locationsList.back().setTheRoot(locationsList.back().getRoot());
		}
		serverList.back().setLocations(locationsList);
	}
}
