/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 12:42:38 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/04 18:23:35 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

Server::Server()
{
	return;
}

Server::Server(std::string str)
{
	setPort(str);
	setName(str);
	setRoot(str);
	setActions(str);
	setMaxClientSize(str);
	setErrorPages(str);
}

Server::Server(const Server &copy)
{
	this->ports = copy.ports;
	this->name = copy.name;
	this->root = copy.root;
	this->index = copy.index;
	this->host = copy.host;
	this->errorPages = copy.errorPages;
	this->maxClientBodySize = copy.maxClientBodySize;
	// this->locations = copy.locations;
	return;
}

Server &Server::operator=(const Server &assign)
{
	this->ports = assign.ports;
	this->name = assign.name;
	this->root = assign.root;
	this->index = assign.index;
	this->host = assign.host;
	this->errorPages = assign.errorPages;
	this->maxClientBodySize = assign.maxClientBodySize;
	// this->locations = assign.locations;
	return *this;
}

Server::~Server(void)
{
	return;
}

void Server::setPort(std::string str)
{
	std::size_t found = str.find("listen") + 6;
	std::string num;
	if (found > str.length())
		raiseError("no port appear in configuration file");
	while (isspace(str[found]))
		found++;
	while (found < str.length())
	{
		if (isdigit(str[found]))
			num.push_back(str[found]);
		else if (isspace(str[found]))
		{
			this->ports.push_back(std::atoi(num.c_str()));
			return;
		}
		found++;
	}
	this->ports.push_back(std::atoi(num.c_str()));
}

std::vector<unsigned int> Server::getPorts(void)
{
	return this->ports;
}

std::string Server::getName(void)
{
	return this->name;
}

void Server::setActions(std::string str)
{
	int flag = 0;
	std::string aux = "server ";
	std::string methods;
	if (str.find("location") < str.length())
		methods = str.substr(0, str.find("location"));
	else
		methods = str;
	std::string word;

	int i = methods.find("limit_except") + 12;
	while (i < static_cast<int>(methods.length()) && i > 12)
	{
		flag++;
		if (isupper(methods[i]))
			word.push_back(methods[i]);
		else if (islower(methods[i]))
			break;
		else
		{
			if (word.length() > 0)
				this->actions.push_back(word);
			word = "";
		}
		i++;
	}
	i = methods.find("allow") + 5;
	while (i < static_cast<int>(methods.length()) && i > 5)
	{
		flag++;
		if (isupper(methods[i]))
			word.push_back(methods[i]);
		else if (islower(methods[i]))
			break;
		else
		{
			if (word.length() > 0)
				this->actions.push_back(word);
			word = "";
		}
		i++;
	}
	if (flag == 0)
	{
		this->actions.push_back("GET");
		this->actions.push_back("POST");
		this->actions.push_back("DELETE");
	}
}

void Server::setName(std::string str)
{
	std::size_t found = str.find("server_name") + 11;
	std::string aux;
	if (found > str.length() || found < 11)
	{
		this->name = "jsaparic";
		return;
	}
	while (isspace(str[found]))
		found++;
	while (found < str.length())
	{
		if (isalpha(str[found]))
			aux.push_back(str[found]);
		else if (isspace(str[found]) || str[found] == ';')
		{
			this->name = aux;
			return;
		}
		found++;
	}
	this->name = aux;
}

void Server::setMaxClientSize(std::string str)
{
	std::size_t found = str.find("client_max_body_size") + 21;
	std::string aux;
	if (found > str.length() || found < 21)
	{
		this->maxClientBodySize = 8000;
		return;
	}
	while (isspace(str[found]))
		found++;
	while (found < str.length())
	{
		if (isdigit(str[found]))
			aux.push_back(str[found]);
		else if (isspace(str[found]) || str[found] == ';')
		{
			this->maxClientBodySize = atoi(aux.c_str());
			return;
		}
		found++;
	}
	this->maxClientBodySize = atoi(aux.c_str());
}

void Server::setRoot(std::string str)
{
	std::size_t found = str.find("root") + 4;
	std::string aux;
	if (found > str.length() || found < 11)
	{
		this->root = "pages/";
		return;
	}
	while (isspace(str[found]))
		found++;
	while (found < str.length())
	{
		if (isalnum(str[found]))
			aux.push_back(str[found]);
		else if (str[found] == '/')
		{
			aux.push_back(str[found]);
			this->root = aux;
			return;
		}
		else if (isspace(str[found]) || str[found] == ';')
		{
			aux.push_back('/');
			this->root = aux;
			return;
		}
		found++;
	}
	aux.push_back('/');
	this->root = aux;
}

std::string Server::getRoot(void)
{
	return this->root;
}

std::list<std::string> Server::getActions(void)
{
	return this->actions;
}

void Server::emptyActions(void)
{
	this->actions.clear();
}

unsigned long Server::getMaxClientSize()
{
	return this->maxClientBodySize;
}

std::vector<Location>	Server::getLocations()
{
	return this->locations;
}

void Server::setErrorPages(std::string str)
{
	std::size_t found = str.find("error_page ") + 11;
	std::string aux;
	std::string num;
	std::list<int> lst;
	if (found > str.length() || found < 11)
		return;
	std::string strcpy = str;
	while (found < strcpy.length() && found > 11)
	{
		std::size_t found = strcpy.find("error_page ") + 11;
		std::string temp = strcpy.substr(found, strcpy.length() - found);
		int i = 0;
		while (temp[i] != ';' && temp[i] != '\n')
		{
			if (isdigit(temp[i]))
				num.push_back(temp[i]);
			else if (isspace(temp[i]) && num.length() > 0)
			{
				lst.push_back(atoi(num.c_str()));
				num.clear();
			}
			else
			{
				while (temp[i] != ';')
				{
					if (isspace(temp[i]) || temp[i] == '\n')
						break;
					aux.push_back(temp[i]);
					i++;
				}
			}
			if (temp[i] != ';')
				i++;
		}
		if (temp[i] == ';' || temp[i] == '\n')
		{
			for (std::list<int>::iterator it = lst.begin(); it != lst.end(); it++)
				this->errorPages.insert(std::pair<int, std::string>(*it, aux));
		}
		aux.clear();
		strcpy = strcpy.substr(i, temp.length() - i);
	}
}

std::map<int, std::string> Server::getErrorPages()
{
	return this->errorPages;
}

void Server::addSocket(const Socket &socket)
{
	this->sockets.push_back(socket);
}

void Server::setLocations(std::vector<Location> &locations)
{
	std::vector<Location> newLocations;
	for (std::vector<Location>::iterator it = locations.begin(); it < locations.end(); it++)
		newLocations.push_back(*it);
	this->locations = newLocations;
}
