/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 20:09:02 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/08 07:58:32 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../../include/Request.hpp"

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

std::string	Request::getMethod()
{
	return this->method;
}

void	Request::setMethod(std::string method)
{
	this->method = method;
}

void	Request::setExtension()
{
	this->extension = this->uri.substr(this->uri.rfind(".") + 1, this->uri.length() - this->uri.rfind("."));
	return;
}

std::string	Request::getExtension()
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

std::string	Request::getUri()
{
	return this->uri;
}

void	Request::setUri(std::string uri)
{
	this->uri = uri;
}

std::string	Request::getVersion()
{
	return this->version;
}

void	Request::setVersion(std::string version)
{
	this->version = version;
}

std::string	Request::getHost()
{
	return this->host;
}

void	Request::setHost(std::string host)
{
	this->host = host;
}

std::string	Request::getUser_agent()
{
	return this->user_agent;
}

void	Request::setUser_agent(std::string user_agent)
{
	this->user_agent = user_agent;
}

std::string	Request::getAccept()
{
	return this->accept;
}

void	Request::setAccept(std::string accept)
{
	this->accept = accept;
}

std::string	Request::getConnection()
{
	return this->connection;
}

void	Request::setConnection(std::string connection)
{
	this->connection = connection;
}
