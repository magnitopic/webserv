/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:42:33 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/04 17:22:55 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

std::vector<Server> parseConfigFile(std::string file_name)
{
	std::string configFile = getFile(file_name);
	std::vector<std::string> directivesServers = splitServerDirectives(configFile);
	int i = 0;

	std::vector<Server> serverList;
	std::vector<Location> locationsList;
	for (std::vector<std::string>::iterator it = directivesServers.begin(); it < directivesServers.end(); it++)
	{
		cout <<"size: "<< sizeof(serverList[0])<< endl;
		std::string aux1 = *it;
		serverList.push_back(Server(aux1));
		cout <<"size: " <<sizeof(serverList[0])<< endl;
		cout << &serverList[0] << endl;
		std::vector<std::string> directivesLocations = splitLocationDirectives(*it);
		for (std::vector<std::string>::iterator it = directivesLocations.begin(); it != directivesLocations.end(); it++){
			Location	*aux = new Location(*it);
			locationsList.push_back(*aux);
		}
		serverList[i].setLocations(locationsList);
		locationsList.clear();
		cout << serverList[0].getLocations().size() << endl;
		i++;
	}
	return serverList;
}
