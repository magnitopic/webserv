/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 19:47:30 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/02 20:05:46 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP_
#define REQUEST_HPP_ 

class Request;

#include "webserv.hpp"

class Request
{
public:
	// atributes
	std::string method;		// GET, POST
	std::string uri;		// route that is requested
	std::string version;	// HTTP/1.1
	std::string host;		// localhost:8080
	std::string user_agent;	// type of browser
	std::string accept;		// type of file that is accepted
	std::string connection;	// keep-alive or close

	// orthodox canon
	Request();
	Request(const Request &copy);
	Request &operator=(const Request &assign);
	~Request();

	// methods
};

#endif
