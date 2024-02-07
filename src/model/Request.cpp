/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 20:09:02 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/07 19:53:22 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../../include/Request.hpp"

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

std::string	Request::getMethod()
{
	return this->method;
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
