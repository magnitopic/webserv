/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 12:42:38 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/30 12:47:35 by jsarabia         ###   ########.fr       */
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

void	Server::setName(std::string str)
{
	std::size_t	found = str.find("server_name") + 11;
	std::string	aux;
	if (found > str.length()){
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
