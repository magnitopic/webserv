/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 20:09:02 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/06 07:58:34 by alaparic         ###   ########.fr       */
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
