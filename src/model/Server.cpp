/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 12:42:38 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/07 14:00:44 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../include/webserv.hpp"

Server::Server()
{
	return;
}

Server::Server(std::string str) : configBuf(str)
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
	*this = copy;
}

Server &Server::operator=(const Server &assign)
{
	if (this != &assign)
	{
		this->ports = assign.ports;
		this->name = assign.name;
		this->root = assign.root;
		this->index = assign.index;
		this->host = assign.host;
		this->errorPages = assign.errorPages;
		this->maxClientBodySize = assign.maxClientBodySize;
		this->locations = assign.locations;
		this->configBuf = assign.configBuf;
	}
	return *this;
}

Server::~Server(void) {}

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

int Server::codeInErrorPages(int code)
{
	for (std::map<int, std::string>::iterator it = this->errorPages.begin(); it != this->errorPages.end(); it++)
	{
		if (it->first == code)
			return 1;
	}
	return 0;
}

void Server::setRoot(std::string str)
{
	std::size_t found = str.find("root") + 4;
	std::string aux;
	if (found > str.length() || found < 4)
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

std::string Server::getTheRoot(void)
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

std::vector<Location> Server::getLocations()
{
	return this->locations;
}

void Server::setErrorPages(std::string str)
{
	std::size_t found = str.find("error_page ");
	std::string strcpy = str;
	while (found != std::string::npos)
	{
		std::size_t start = found + 11;
		std::size_t end = str.find(";", start);
		if (end == std::string::npos)
			break;
		std::string line = str.substr(start, end - start);
		std::istringstream iss(line);

		std::string aux;
		std::istringstream tokens(line);
		std::vector<std::string> values;
		while (tokens >> aux)
			values.push_back(aux);

		if (values.size() < 2)
			raiseError("error_page directive is missing values");

		// we save each element as int, except the last element that is the value of each key
		std::vector<int> numKeys;
		for (std::vector<std::string>::iterator it = values.begin(); it != values.end() - 1; it++)
		{
			int val = 0;
			std::stringstream convert(*it);
			convert >> val;
			if (convert.fail())
				raiseError("Wrong formatting in error_page directive");
			numKeys.push_back(val);
		}

		for (std::vector<int>::iterator it = numKeys.begin(); it != numKeys.end(); it++)
		{
			if (this->errorPages.find(*it) != this->errorPages.end())
				raiseError("error_page directive is duplicated");
			this->errorPages.insert(std::pair<int, std::string>(*it, values.back()));
		}
		//  check if there are any `more error_page` directives left
		str = str.substr(end, str.length() - 1);
		found = str.find("error_page ");
	}
}

std::string Server::getConfigBuf()
{
	return this->configBuf;
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
	this->locations.clear();
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		this->locations.push_back(Location(*it));
	}
}
