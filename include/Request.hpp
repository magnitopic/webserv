/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 19:47:30 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/08 07:56:26 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP_
#define REQUEST_HPP_

class Request;

#include "webserv.hpp"

class Request
{
private:
	// atributes
	std::string	method;		// GET, POST
	std::string	uri;		// route that is requested
	std::string	version;	// HTTP/1.1
	std::string	host;		// localhost:8080
	std::string	user_agent; // type of browser
	std::string	accept;		// type of file that is accepted
	std::string	connection; // keep-alive or close
	std::string	extension;
	std::string	contentType;

public:
	// orthodox canonical form

	Request();
	Request(const Request &copy);
	Request &operator=(const Request &assign);
	~Request();
	// setters and getters

	std::string	getMethod();
	void		setMethod(std::string method);
	std::string	getUri();
	void		setUri(std::string uri);
	std::string	getVersion();
	void		setVersion(std::string version);
	std::string	getHost();
	void		setHost(std::string host);
	std::string	getUser_agent();
	void		setUser_agent(std::string user_agent);
	std::string	getAccept();
	void		setAccept(std::string accept);
	std::string	getConnection();
	void		setConnection(std::string connection);
	void		setExtension();
	std::string	getExtension();
	void		setContentType(std::string type);
	std::string	getContentType(void);
	// methods
};

#endif
