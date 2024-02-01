/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 18:49:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/01 17:56:53 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Socket.hpp"
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
	this->response = socket.response;
	return;
}

Socket &Socket::operator=(const Socket &socket)
{
	this->directory = socket.directory;
	this->actions = socket.actions;
	this->response = socket.response;
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

void	Socket::setHeader(std::string header){
	this->header = header;
	return;
}

void	Socket::setResponse(std::string response){
	this->response = response;
	return;
}

std::string Socket::getDirectory(void)
{
	return this->directory;
}

std::string Socket::getResponse(void)
{
	return this->response;
}

std::string	Socket::getHeader(void){
	return this->header;
}

void	Socket::setActions(std::string directory, std::string text){
	std::string aux = "location " + directory;
	if (text.find(aux) >= text.length())
		return;
	std::string methods = text.substr(text.find(aux), text.find("}") - text.find(aux));
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

std::string &Socket::getActionsArray(int i)
{
	std::list<std::string>::iterator it = this->actionsArr.begin();
	for (int n = 0; n < i; n++)
		it++;
	std::string &aux = *it;
	return aux;
}

void	Socket::setContentLength(std::string cont)
{
	int size = static_cast<int>(cont.length());
	std::string aux = std::to_string(size);
	const char *str = aux.c_str();
	this->contentLength = str;
}

std::string	Socket::getContentLength(void)
{
	return this->contentLength;
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

void servePages()
{
}

void Socket::generateAutoIndex(Server& server, std::string route, Socket& socketClass)
{
	std::string finalRoute = server.getRoot() + route;
	if (access(finalRoute.c_str(), R_OK) != 0)
		return;
	DIR *dirContents;
	dirContents = opendir(finalRoute.c_str());
	if (!dirContents)
		raiseError("openDir failled");
	struct dirent *entry = readdir(dirContents);
	std::string name = entry->d_name;

	// TODO: this should be seperated into diferent functions
	std::string page = "<head><title>Index of " + route + "</title></head>";
	page += "<body><h1>Index of " + route + "</h1>";
	while (entry != NULL)
	{
		name = entry->d_name;
		page += "<a href=" + route + "/" + entry->d_name + ">" + entry->d_name + "</a><br>";
		std::cout << entry->d_name << std::endl;
		entry = readdir(dirContents);
}
	page += "<p>Proudly served by alaparic and jsarabia.</p></body></html>";
	socketClass.setResponse(page);
	closedir(dirContents);
}

std::ostream &operator<<(std::ostream &os, std::list<std::string> &list)
{
	for (std::list<std::string>::iterator it = list.begin(); it != list.end(); it++)
		os << *it << " ";
	return os;
}
