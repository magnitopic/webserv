/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 18:12:16 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/08 07:52:49 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

class Response;

#include "webserv.hpp"
#include <string>

class Response
{
private:
	// atributes

	std::map<int, std::string>	errorCode;
	std::string					response;
	int							contentLength;
	std::string					header;

public:
	// orthodox canonical form

	Response();
	Response(const Response &copy);
	Response &operator=(const Response &assign);
	~Response();
	// setters and getters

	std::string	getErrorMsg(int id);
	void		generateResponse(int code, std::string def, Server& server);
	std::string	getResponse(void);
	void		setContentLength(std::string cont);
	int			getContentLength(void);
	void		generateHeader(int code, std::string def, Server& server);
	std::string	getHeader(void);
	void		setResponse(std::string response);
	void		generateHeaderContent(int code, std::string type, Server& server);
	void		setResponseNotFound();
	// methods

	std::string generateHttpResponse(std::string contentType);
};

#endif
