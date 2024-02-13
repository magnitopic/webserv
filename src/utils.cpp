/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:46:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/13 11:49:28 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/webserv.hpp"

void raiseError(const char *msg)
{
	perror(msg);
	exit(1);
}

int	myOwnFind(Location &location, Request &req)
{
	std::list<std::string> aux = location.getActions();
	for (list<string>::iterator it = aux.begin(); it != aux.end(); it++)
	{
		if (*it == req.getMethod())
			return 1;
	}
	return 0;
}

int	myOwnFindServ(Server &location, Request &req)
{
	std::list<std::string> aux = location.getActions();
	for (list<string>::iterator it = aux.begin(); it != aux.end(); it++){
		if (*it == req.getMethod())
			return 1;
	}
	return 0;
}

std::string	configToString(char *str)
{
	std::ifstream	configFile;
	std::string		file;

	configFile.open(str, std::ifstream::in);
	if (!configFile.is_open())
		raiseError("error opening the input file");
	while (configFile.good()){
		char c = configFile.get();
		file.push_back(c);
	}
	configFile.close();
	return file;
}

int	isAllowed(Server &server, Request &req, Location &location)
{
	if (location.getForbidden().size() > 1){
		std::list<std::string>::iterator it = std::find(location.getForbidden().begin(), (std::prev(location.getForbidden().end())), req.getMethod());
		if (it != location.getForbidden().end())
			return 0;
	}
	else if (location.getForbidden().size() == 1){
		if (req.getMethod() == *location.getForbidden().begin())
			return 0;
	}
	if (location.getActions().size() > 1){
		int it = myOwnFind(location, req);
		if (it)
			return 1;
	}
	else if (location.getActions().size() == 1){
		if (req.getMethod() == *location.getActions().begin())
			return 1;
	}
	if (server.getActions().size() > 1){
		int it = myOwnFindServ(server, req);
		if (it)
			return 1;
	}
	else if (server.getActions().size() == 1){
		if (req.getMethod() == *server.getActions().begin())
			return 1;
	}
	return 0;
}


std::string deleteFirstElement(std::string str)
{
	reverse(str.begin(), str.end());
	str.pop_back();
	reverse(str.begin(), str.end());
	return str;
}

//void	setIndex(finalRoute, socketClass.getDirectory())
