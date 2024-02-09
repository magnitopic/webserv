/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 18:12:16 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/09 20:30:03 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_


#include "webserv.hpp"
#include <string>

/**
 * The aim of this class is to store the information that will eventually make
 * up the response given to the client.
*/
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
	std::string	getResponse(void);
	void		setContentLength(std::string cont);
	int			getContentLength(void);
	std::string	getHeader(void);
	void		setResponse(std::string response);
	void		setResponseNotFound();
	// methods

	void		setResponseHTML(int code);
	void		setResponseForbidden();
	std::string generateHttpResponse();
	void		generateHeader(int code, Server& server);
	void		generateHeaderContent(int code, std::string type, Server& server);
	void		generateResponse(int code, std::string def, Server& server);
};

#endif
