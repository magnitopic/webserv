/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:42:33 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/04 18:46:15 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

void parseConfigFile(std::string file_name, std::vector<Server> &serverList)
{
	std::string configFile = getFile(file_name);
	std::vector<std::string> directivesServers = splitServerDirectives(configFile);
	int i = 0;

	std::vector<std::vector<Location> > finalLocs;
	for (std::vector<std::string>::iterator it = directivesServers.begin(); it < directivesServers.end(); it++)
	{
		std::string aux1 = *it;
		serverList.push_back(Server(aux1));
		std::vector<std::string> directivesLocations = splitLocationDirectives(aux1);
		std::vector<Location> locationsList;
		for (std::vector<std::string>::iterator it2 = directivesLocations.begin(); it2 != directivesLocations.end(); it2++)
		{
			Location aux = (*it2);
			locationsList.push_back(aux);
			locationsList.back().setTheRoot(aux.getRoot());
			cout << "aux: " << aux.getRoot() << endl;
			cout << "back: " << locationsList.back().getRoot() << endl;
		}
		serverList.back().setLocations(locationsList);
		finalLocs.push_back(locationsList);
		i++;
	}
	i = 0;
	for (std::vector<std::vector<Location> >::iterator it3 = finalLocs.begin(); it3 != finalLocs.end(); it3++)
	{
		(serverList[i]).setLocations(*it3);
		cout << "pero: " << serverList[i].getLocations().size() << endl;
		i++;
	}
	cout << "holiiii" << endl;
	for (std::vector<Location>::iterator it = serverList[0].getLocations().begin(); it != serverList[0].getLocations().end(); it++)
		cout << (*it).getRoot() << endl;
	exit(0);
}
