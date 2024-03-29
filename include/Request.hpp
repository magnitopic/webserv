/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 19:47:30 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/07 15:57:51 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include "webserv.hpp"

/**
 * The aim of this class is to store the information provided by the request the
 * client made. This information will be used to check wether the request can be
 * fulfilled or not, and serve to see what type of response will be given to the
 * client.
 */
class Request
{
private:
	// atributes
	std::string	reqBuffer;	// request made by the client
	std::string	method;		// GET, POST
	std::string	uri;		// route that is requested
	std::string	absPath;	// root + uri
	std::string	version;	// HTTP/1.1
	std::string	host;		// localhost:8080
	std::string	user_agent; // type of browser
	std::string	accept;		// type of file that is accepted
	std::string	connection; // keep-alive or close
	std::string	extension;
	std::string	contentType;
	std::string	postType;
	std::string	getArgs;	// arguments received in case of a GET request with a query ("url?value1&value2")
	std::string	originalUri;
	int			contentLength;
	int			clientBodySize;	// the maximum size of the body send by the client
	int			port;

public:
	// orthodox canonical form

	Request();
	Request(const Request &copy);
	Request &operator=(const Request &assign);
	~Request();
	// setters and getters

	void		setReqBuffer(std::string buffer);
	std::string	getReqBuffer();
	void		setContentLength();
	int			getContentLength();
	std::string	getMethod();
	void		setMethod(std::string method);
	std::string	getUri();
	void		setUri(std::string uri);
	std::string	getVersion();
	void		setVersion(std::string version);
	std::string	getHost();
	void		setHost();
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
	void		setAbsPath(Server& server);
	std::string	getAbsPath();
	int			getClientBodySize();
	void		setClientBodySize(int maxSize);
	void		handleSlash();
	void		setPort();
	int			getPort();
	void		newSetPort(unsigned int nport);
	std::string	getterGetArgs();
	std::string	getOriginalUri();
	// methods

	Request		parseReq(std::string passedReq);
	void		fixURI(Server& server);
};

#endif
