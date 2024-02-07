/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 19:47:30 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/07 19:29:00 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP_
#define REQUEST_HPP_

class Request;

#include "webserv.hpp"

class Request
{
//private:
public:
	// atributes
	std::string	method;		// GET, POST
	std::string uri;		// route that is requested
	std::string version;	// HTTP/1.1
	std::string host;		// localhost:8080
	std::string user_agent;	// type of browser
	std::string accept;		// type of file that is accepted
	std::string connection;	// keep-alive or close
	std::string	extension;
	std::string	contentType;

	// orthodox canon
	Request();
	Request(const Request &copy);
	Request &operator=(const Request &assign);
	~Request();
	std::string	getMethod();
	void		setExtension();
	std::string	getExtension();
	void		setContentType(std::string type);
	std::string	getContentType(void);

	// methods
};

#endif
