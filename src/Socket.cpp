/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 18:49:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/07 19:57:48 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

Socket::Socket(void)
{
	this->actionsArr.push_back("GET");
	this->actionsArr.push_back("POST");
	this->actionsArr.push_back("DELETE");
	return;
}

Socket::Socket(std::string directory) : directory(directory)
{
	return;
}

Socket::Socket(const Socket &socket)
{
	this->directory = socket.directory;
	this->actions = socket.actions;
	return;
}

Socket &Socket::operator=(const Socket &socket)
{
	this->directory = socket.directory;
	this->actions = socket.actions;
	return *this;
}

Socket::~Socket(void)
{
	return;
}

// setters & getters

void Socket::setDirectory(std::string directory)
{
	this->directory = directory;
	return;
}


std::string Socket::getDirectory(void)
{
	return this->directory;
}


void Socket::setActions(Server& server, std::string directory, std::string text)
{
	std::string aux = "location " + directory;
	if (text.find(aux) >= text.length())
		return;
	std::string methods = text.substr(text.find(aux), text.find("}") - text.find(aux));
	std::string word;
	int i = methods.find("limit_except") + 12;
	while (i < static_cast<int>(methods.length()) || i < 12)
	{
		server.emptyActions();
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

void Socket::setForbidden(std::string directory, std::string text)
{
	std::string aux = "location " + directory;
	if (text.find(aux) >= text.length())
		return;
	std::string methods = text.substr(text.find(aux), text.find("}") - text.find(aux));
	std::string word;
	int i = methods.find("deny") + 4;
	while (i < static_cast<int>(methods.length()))
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

std::list<std::string> Socket::getActions(void)
{
	return this->actions;
}

std::string Socket::getRoot(void)
{
	return this->root;
}

bool Socket::getAutoIndex(void)
{
	return this->autoIndex;
}

void Socket::setAutoIndex(bool autoIndex)
{
	this->autoIndex = autoIndex;
}

std::list<std::string>	Socket::getForbidden(void)
{
	return this->forbidden;
}

// methods

bool isAutoindex(std::string str, Socket socketClass)
{
	std::string aux = "location " + socketClass.getDirectory() + " ";
	if (str.find(aux) >= str.length())
		return false;
	std::string methods = str.substr(str.find(aux), str.find("}") - str.find(aux));
	std::string word;
	unsigned int i = methods.find("autoindex") + 10;
	if (i > methods.length())
		return false;
	word.push_back(methods[i]);
	word.push_back(methods[i + 1]);
	if (word == "on")
		return true;
	return false;
}

static void servePages(std::string route, dirent *entry, DIR* dirContents, Response& response)
{
	std::string page = "<head><title>Index of " + route + "</title></head>";
	page += "<body><h1>Index of " + route + "</h1>";
	while (entry != NULL)
	{
		page += "<a href=" + route + "/" + entry->d_name + ">" + entry->d_name + "</a><br>";
		entry = readdir(dirContents);
	}
	page += "<p>Proudly served by alaparic and jsarabia.</p></body></html>";
	response.setResponse(page);
	closedir(dirContents);
}

void Socket::generateAutoIndex(Server &server, std::string route, Socket &socketClass, Response &response)
{
	std::string finalRoute;
	if (socketClass.getDirectory()[0] != '/' || server.getRoot()[server.getRoot().length() - 1] != '/')
		finalRoute = server.getRoot() + socketClass.getDirectory();
	else
		finalRoute = server.getRoot() + socketClass.getDirectory().substr(1, socketClass.getDirectory().length() - 1);
	if (finalRoute[finalRoute.length() - 1] == '/')
		finalRoute.pop_back();
	if (access(finalRoute.c_str(), R_OK) != 0)
		return;
	DIR *dirContents;
	dirContents = opendir(finalRoute.c_str());
	if (!dirContents)
		raiseError("openDir failled");
	struct dirent *entry = readdir(dirContents);
	if (route[route.length() - 1] == '/')
		route.pop_back();

	// TODO: this should be seperated into diferent functions
	servePages(route, entry, dirContents, response);
}

/* *
 * Combines the values for the response to the client, joins them together and returns
 * the string that will be sent to the clinet as a response
 */

std::ostream &operator<<(std::ostream &os, std::list<std::string> list)
{
	for (std::list<std::string>::iterator it = list.begin(); it != list.end(); it++)
		os << *it << " ";
	return os;
}

void	Socket::emptyActions(void)
{
	this->actions.clear();
}
