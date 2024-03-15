/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:46:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/15 16:34:06 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

std::string	vectorToString(std::vector<char> vector)
{
	std::string	str;
	for (std::vector<char>::iterator it = vector.begin(); it != vector.end(); it++)
		str.push_back(*it);
	return str;
}

std::string bodyReq(std::string str)
{
	size_t pos = str.find("\r\n\r\n") + 4;
	if (pos < 4 || pos > str.length())
	{
		if (!strncmp(str.substr(0, 4).c_str(), "POST", 4))
			return "";
		else
			return str;
	}
	return str.substr(pos, str.length() - pos);
}

void raiseError(const char *msg)
{
	std::cerr << RED << std::endl;
	perror(msg);
	std::cerr << RESET << std::endl;
	exit(1);
}

int myOwnFind(std::list<std::string> lst, Request &req)
{
	std::list<std::string> aux = lst;
	for (list<string>::iterator it = aux.begin(); it != aux.end(); it++)
	{
		if (*it == req.getMethod())
			return 1;
	}
	return 0;
}

std::string configToString(char *str)
{
	std::ifstream configFile;
	std::string file;

	configFile.open(str, std::ifstream::in);
	if (!configFile.is_open())
		raiseError("error opening the input file");
	while (configFile.good())
	{
		char c = configFile.get();
		file.push_back(c);
	}
	configFile.close();
	return file;
}

int isAllowed(Server &server, Request &req, Location &location)
{
	if (location.getForbidden().size() > 1)
	{
		int it = myOwnFind(location.getForbidden(), req);
		if (it)
			return 0;
	}
	else if (location.getForbidden().size() == 1)
	{
		if (req.getMethod() == *location.getForbidden().begin())
			return 0;
	}
	if (location.getActions().size() > 1)
	{
		int it = myOwnFind(location.getActions(), req);
		if (it)
			return 1;
	}
	else if (location.getActions().size() == 1)
	{
		if (req.getMethod() == *location.getActions().begin())
			return 1;
	}
	if (server.getActions().size() > 1)
	{
		int it = myOwnFind(server.getActions(), req);
		if (it)
			return 1;
	}
	else if (server.getActions().size() == 1)
	{
		if (req.getMethod() == *server.getActions().begin())
			return 1;
	}
	return 0;
}

std::string deleteFirstElement(std::string str)
{
	reverse(str.begin(), str.end());
	str.erase(str.size() - 1);
	reverse(str.begin(), str.end());
	return str;
}

// void	setIndex(finalRoute, socketClass.getDirectory())

std::map<int, std::string>::const_iterator getMapIterator(const std::map<int, std::string> &map, int code)
{
	for (std::map<int, std::string>::const_iterator it = map.begin(); it != map.end(); it++)
	{
		if (it->first == code)
			return it;
	}
	return map.end();
}

int myOwnFindVector(std::vector<unsigned int> vector, unsigned int num)
{
	std::vector<unsigned int> aux = vector;
	for (std::vector<unsigned int>::iterator it = aux.begin(); it != aux.end(); it++)
	{
		if (*it == num)
			return 1;
	}
	return 0;
}

int fixingCPP(std::vector<Server> &servers, Request &req)
{
	int i = 0;

	unsigned int temp = *servers[i].getPorts().begin();
	for (i = 0; i < static_cast<int>(servers.size()); i++)
	{
		temp = *servers[i].getPorts().begin();
		if (!strncmp(servers[i].getName().c_str(), req.getHost().c_str(), req.getHost().length()) && servers[i].getPorts().size() > 1 && std::find(servers[i].getPorts().begin(), servers[i].getPorts().end(), req.getPort()) != servers[i].getPorts().end())
			break;
		else if (!strncmp(servers[i].getName().c_str(), req.getHost().c_str(), req.getHost().length()) && servers[i].getPorts().size() == 1 && temp == static_cast<unsigned int>(req.getPort()))
			break;
		else if (!strncmp(servers[i].getName().c_str(), req.getHost().c_str(), req.getHost().length()) && req.getPort() == 0)
			break;
	}
	return i;
}

int fixingCPPAgain(std::vector<Server> &servers, Request &req)
{
	int i = 0;

	unsigned int temp;
	for (i = 0; i < static_cast<int>(servers.size()); i++)
	{
		temp = *servers[i].getPorts().begin();
		if (servers[i].getPorts().size() > 1 && myOwnFindVector(servers[i].getPorts(), req.getPort()))
			break;
		else if (servers[i].getPorts().size() == 1 && temp == static_cast<unsigned int>(req.getPort()))
			break;
	}
	return i;
}
