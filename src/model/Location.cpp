/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 11:58:52 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/08 17:29:30 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

Location::Location(void)
{
	this->actionsArr.push_back("GET");
	this->actionsArr.push_back("POST");
	this->actionsArr.push_back("DELETE");
	return;
}

Location::Location(std::string directory) : directory(directory)
{
	this->index = "index.html";
	return;
}

Location::Location(const Location &location)
{
	this->directory = location.directory;
	this->actions = location.actions;
	this->index = location.index;
	return;
}

Location &Location::operator=(const Location &location)
{
	this->directory = location.directory;
	this->actions = location.actions;
	this->index = location.index;
	return *this;
}

Location::~Location(void)
{
	return;
}

// setters & getters

void Location::setDirectory(std::string directory)
{
	this->directory = directory;
	return;
}


std::string Location::getDirectory(void)
{
	return this->directory;
}


void Location::setActions(Server& server, std::string text)
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

void Location::setForbidden()
{
	if (buffer.length() < 1)
		return;
	std::string word;
	int i = buffer.find("deny") + 4;
	while (i < static_cast<int>(buffer.length()))
	{
		if (isupper(buffer[i]))
			word.push_back(buffer[i]);
		else if (islower(buffer[i]))
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

std::list<std::string> Location::getActions(void)
{
	return this->actions;
}

std::string Location::getRoot(void)
{
	return this->root;
}

bool Location::getAutoIndex(void)
{
	return this->autoIndex;
}

void Location::setAutoIndex(bool autoIndex)
{
	this->autoIndex = autoIndex;
}

std::list<std::string>	Location::getForbidden(void)
{
	return this->forbidden;
}

// methods

bool isAutoindex(Location &location)
{
	if (location.getBuffer().length() < 1)
		return false;
	std::string word;
	unsigned int i = location.getBuffer().find("autoindex") + 10;
	if (i > location.getBuffer().length())
		return false;
	word.push_back(location.getBuffer()[i]);
	word.push_back(location.getBuffer()[i + 1]);
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

void Location::generateAutoIndex(Server &server, std::string route, Location &location, Response &response)
{
	std::string finalRoute;
	if (location.getDirectory()[0] != '/' || server.getRoot()[server.getRoot().length() - 1] != '/')
		finalRoute = server.getRoot() + location.getDirectory();
	else
		finalRoute = server.getRoot() + location.getDirectory().substr(1, location.getDirectory().length() - 1);
	if (finalRoute[finalRoute.length() - 1] == '/')
		finalRoute.pop_back();
	if (access(finalRoute.c_str(), R_OK) != 0)
		return;
	DIR *dirContents;
	dirContents = opendir(finalRoute.c_str());
	if (!dirContents)
		raiseError("openDir failed");
	struct dirent *entry = readdir(dirContents);
	if (route[route.length() - 1] == '/')
		route.pop_back();

	// TODO: this should be separated into different functions
	servePages(route, entry, dirContents, response);
}

/* *
 * Combines the values for the response to the client, joins them together and returns
 * the string that will be sent to the client as a response
 */

std::ostream &operator<<(std::ostream &os, std::list<std::string> list)
{
	for (std::list<std::string>::iterator it = list.begin(); it != list.end(); it++)
		os << *it << " ";
	return os;
}

void	Location::emptyActions(void)
{
	this->actions.clear();
}

void	Location::setBuffer(std::string configFile)
{
	std::string aux = "location " + directory + " ";
	if (configFile.find(aux) >= configFile.length()){
		this->buffer = "";
		return;
	}
	int len = aux.size() + 1;
	std::string temp = configFile.substr(configFile.find(aux) + len, configFile.length() - configFile.find(aux));
	this->buffer = temp.substr(0, temp.find("}"));
}

std::string	Location::getBuffer(void)
{
	return this->buffer;
}

void	Location::setIndex()
{
	this->index = this->directory + "/index.html";
	if (buffer.length() < 1)
		return;
	std::string word;
	unsigned int i = buffer.find("index ") + 6;
	if (i > buffer.length() || (buffer[i - 7] && isalnum(buffer[i - 7])))
		return;
	std::string temp = buffer.substr(i, buffer.length() - i);
	word = temp.substr(0, temp.find(';'));
	this->index = directory + "/" + word;
}

std::string	Location::getIndex()
{
	return this->index;
}
