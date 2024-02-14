/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:42:33 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/14 15:39:39 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../include/webserv.hpp"

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
