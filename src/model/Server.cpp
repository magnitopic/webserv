/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 12:42:38 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/03 16:12:45 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

Server::Server(){
	return;
}

Server::Server(const Server &copy)
{
	this->port = copy.port;
	this->name = copy.name;
	this->root = copy.root;
	this->index = copy.index;
	this->host = copy.host;
	this->errorPages = copy.errorPages;
	this->maxClientBodySize = copy.maxClientBodySize;
	//this->locations = copy.locations;
	return;
}

Server&	Server::operator=(const Server &assign)
{
	this->port = assign.port;
	this->name = assign.name;
	this->root = assign.root;
	this->index = assign.index;
	this->host = assign.host;
	this->errorPages = assign.errorPages;
	this->maxClientBodySize = assign.maxClientBodySize;
	//this->locations = assign.locations;
	return *this;
}

Server::~Server(void)
{
	return;
}

void	Server::setPort(std::string str)
{
	std::size_t	found = str.find("listen") + 6;
	std::string	num;
	if (found > str.length())
		raiseError("no port appear in configuration file");
	while (isspace(str[found]))
		found++;
	while (found < str.length()){
		if (isdigit(str[found]))
			num.push_back(str[found]);
		else if (isspace(str[found])){
			this->port = std::atoi(num.c_str());
			return;
		}
		else
			raiseError("error in configuration file");
		found++;
	}
	this->port = std::atoi(num.c_str());
}

int	Server::getPort(void)
{
	return this->port;
}

std::string	Server::getName(void)
{
	return this->name;
}

void	Server::setActions(std::string str)
{
	std::string aux = "server ";
	if (str.find(aux) >= str.length())
		return;
	std::string methods = str.substr(str.find(aux), str.find("location") - str.find(aux));
	std::string word;
	int i = methods.find("limit_except") + 12;
	while (i < static_cast<int>(methods.length()) || i < 12)
	{
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
	while (i < static_cast<int>(methods.length()) || i < 5)
	{
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
}

void	Server::setName(std::string str)
{
	std::size_t	found = str.find("server_name") + 11;
	std::string	aux;
	if (found > str.length() || found < 11){
		this->name = "jsaparic";
		return;
	}
	while (isspace(str[found]))
		found++;
	while (found < str.length()){
		if (isalpha(str[found]))
			aux.push_back(str[found]);
		else if (isspace(str[found]) || str[found] == ';'){
			this->name = aux;
			return;
		}
		found++;
	}
	this->name = aux;
}

void	Server::setRoot(std::string str)
{
	std::size_t	found = str.find("root")  +4;
	std::string	aux;
	if (found > str.length() || found < 11){
		this->root = "pages/";
		return;
	}
	while (isspace(str[found]))
		found++;
	while (found < str.length()){
		if (isalnum(str[found]))
			aux.push_back(str[found]);
		else if (str[found] == '/'){
			aux.push_back(str[found]);
			this->root = aux;
			return;
		}
		else if (isspace(str[found]) || str[found] == ';'){
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

void	Server::emptyActions(void)
{
	this->actions.clear();
}
