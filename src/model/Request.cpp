/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 20:09:02 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/15 15:14:04 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

// orthodox canonical form

Request::Request() {}

Request::Request(const Request &copy)
{
	*this = copy;
}

Request &Request::operator=(const Request &assign)
{
	if (this != &assign)
	{
		this->method = assign.method;
		this->uri = assign.uri;
		this->version = assign.version;
		this->host = assign.host;
		this->user_agent = assign.user_agent;
		this->accept = assign.accept;
		this->connection = assign.connection;
	}
	return *this;
}

Request::~Request() {}

// setters and getters

std::string Request::getMethod()
{
	return this->method;
}

void Request::setMethod(std::string method)
{
	this->method = method;
}

void Request::setExtension()
{
	this->extension = this->uri.substr(this->uri.rfind(".") + 1, this->uri.length() - this->uri.rfind("."));
	return;
}

std::string Request::getExtension()
{
	return this->extension;
}

std::string Request::getContentType()
{
	return this->contentType;
}

void Request::setContentType(std::string type)
{
	this->contentType = type;
}

std::string Request::getUri()
{
	return this->uri;
}

void Request::setUri(std::string uri)
{
	this->uri = uri;
}

std::string Request::getVersion()
{
	return this->version;
}

void Request::setVersion(std::string version)
{
	this->version = version;
}

std::string Request::getHost()
{
	return this->host;
}

void Request::setHost()
{
	size_t pos = this->reqBuffer.find("Host:") + 5;
	std::string temp;
	if (pos < 5 || pos > this->reqBuffer.length() + 5)
		this->host = "localhost";
	else
	{
		while (isspace(this->reqBuffer[pos]))
			pos++;
		while (this->reqBuffer[pos] != '\n' && this->reqBuffer[pos] != ':' && this->reqBuffer[pos] != '\r')
		{
			temp.push_back(this->reqBuffer[pos]);
			pos++;
		}
	}
	this->host = temp;
}

std::string Request::getUser_agent()
{
	return this->user_agent;
}

void Request::setUser_agent(std::string user_agent)
{
	this->user_agent = user_agent;
}

std::string Request::getAccept()
{
	return this->accept;
}

void Request::setAccept(std::string accept)
{
	this->accept = accept;
}

std::string Request::getConnection()
{
	return this->connection;
}

void Request::setConnection(std::string connection)
{
	this->connection = connection;
}

void Request::setAbsPath(Server &server)
{
	this->absPath = server.getTheRoot();
	if (this->uri[0] == '/' && absPath[absPath.length() - 1] == '/')
		this->absPath.erase(this->absPath.size() - 1);
	if (this->uri.find("?") < this->uri.length() && this->uri.find("?") >= 0)
	{
		this->absPath += this->uri.substr(0, this->uri.find("?"));
		this->getArgs = this->uri.substr(this->uri.find("?"), this->uri.length() - this->uri.find("?"));
		return;
	}
	if (this->uri[0] == '/' && this->absPath.back() == '/')
		this->absPath.erase(this->absPath.size() - 1);
	this->absPath += this->uri;
	if (this->absPath.back() == '/')
		this->absPath.erase(this->absPath.size() - 1);
}

std::string Request::getterGetArgs()
{
	return this->getArgs;
}

std::string Request::getAbsPath()
{
	return std::string(this->absPath);
}
// methods

/**
 * Parses the request and stores the information in the Request object.
 */
Request Request::parseReq(std::string passedReq)
{
	std::istringstream iss(passedReq);
	std::string line;
	int count = 0;
	Request req;

	while (std::getline(iss, line))
	{
		std::istringstream iss2(line);
		std::string word;
		while (std::getline(iss2, word, ' '))
		{
			switch (count)
			{
			case 0:
				req.setMethod(word);
				break;
			case 1:
				req.setUri(word);
				break;
			case 2:
				req.setVersion(word);
				break;
			}
			count++;
		}
	}
	return req;
}

void Request::setContentLength()
{
	int pos = this->reqBuffer.find("Content-Length:") + 15;
	if (pos >= static_cast<int>(this->reqBuffer.length()) || pos < 15)
	{
		this->contentLength = 0;
		return;
	}
	std::istringstream temp(this->reqBuffer.substr(pos, this->reqBuffer.length() - pos));
	std::string num;
	std::getline(temp, num);
	this->contentLength = std::atoi(num.c_str());
}
int Request::getContentLength()
{
	return this->contentLength;
}

void Request::setReqBuffer(std::string buffer)
{
	this->reqBuffer = buffer;
}

std::string Request::getReqBuffer()
{
	return this->reqBuffer;
}

int Request::getClientBodySize()
{
	return this->clientBodySize;
}

void Request::setClientBodySize(int maxSize)
{
	this->clientBodySize = maxSize;
}

void Request::handleSlash()
{
	if (this->absPath.back() == '/')
		this->absPath.erase(this->absPath.size() - 1);
	if (this->uri.back() == '/')
		this->uri.erase(this->uri.size() - 1);
}

void Request::setPort()
{
	size_t pos = this->reqBuffer.find(this->host) + this->host.length();
	if (pos < this->host.length() || pos > this->reqBuffer.length())
	{
		raiseError("Unexpected error");
	}
	std::string num;
	while (this->reqBuffer[pos] == ':')
		pos++;
	while (pos < this->reqBuffer.length())
	{
		if (isdigit(this->reqBuffer[pos]))
			num.push_back(this->reqBuffer[pos]);
		else
			break;
		pos++;
	}
	this->port = std::atoi(num.c_str());
}

int Request::getPort()
{
	return this->port;
}

void Request::newSetPort(unsigned int nport)
{
	this->port = nport;
}

void Request::fixURI(Server &server)
{
	this->originalUri = this->uri;
	std::string str = this->uri;
	std::string newUri = "/";
	char *pch;
	pch = strtok(const_cast<char *>(str.c_str()), "/");
	while (pch != NULL)
	{
		std::vector<Location> aux = server.getLocations();
		std::string aux2 = "/";
		aux2 += pch;
		int i = 0;
		for (std::vector<Location>::iterator it = aux.begin(); it != aux.end(); it++)
		{
			if ((*it).getRoot().length() > 0 && !strncmp(aux2.c_str(), (*it).getDirectory().c_str(), (*it).getDirectory().length()))
			{
				i = 1;
				if ((*it).getRoot().length() > 0)
				{
					if (!strncmp((*it).getRoot().c_str(), "./", (*it).getRoot().length()))
						break;
					else if (!strncmp((*it).getRoot().c_str(), "../", (*it).getRoot().length()))
					{
						if (aux2.back() == '/')
							aux2.erase(aux2.size() - 1);
					}
					if (newUri.back() == '/' && (*it).getRoot()[0] == '/')
						newUri.erase(newUri.size() - 1);
					newUri += (*it).getRoot();
				}
				else
				{
					if (newUri.back() == '/' && pch[0] == '/')
						newUri.erase(newUri.size() - 1);
					newUri += pch;
				}
			}
			// memset(pch, 0, strlen(pch));
		}
		if (i == 0)
		{
			newUri += pch;
			newUri += "/";
		}
		if (newUri.back() == '/' && pch[0] == '/')
			newUri.erase(newUri.size() - 1);
		pch = strtok(NULL, "/");
	}
	if (newUri[0] == '/' && newUri[1] == '/')
		newUri = deleteFirstElement(newUri);
	if (newUri.length() > 0)
		this->uri = newUri;
}

std::string Request::getOriginalUri()
{
	return this->originalUri;
}