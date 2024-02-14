/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:42:33 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/14 17:40:59 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../include/webserv.hpp"

std::vector<Server> parseConfigFile(std::string file_name)
{
	std::string configFile = getFile(file_name);
	std::vector<std::string> directivesServers = splitServerDirectives(configFile);

	std::vector<Server> serverList;
	for (std::vector<std::string>::iterator it = directivesServers.begin(); it < directivesServers.end(); it++)
	{
		serverList.push_back(Server(*it));
		std::vector<std::string> directivesLocations = splitLocationDirectives(*it);
		std::vector<Location> locationsList;
		for (std::vector<std::string>::iterator it2 = directivesLocations.begin(); it2 < directivesLocations.end(); it2++)
			locationsList.push_back(Location(*it2));
		serverList[serverList.size() - 1].setLocations(locationsList);
	}
	return serverList;
}
