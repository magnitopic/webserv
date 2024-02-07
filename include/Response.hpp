/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 18:12:16 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/07 19:48:41 by jsarabia         ###   ########.fr       */
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
	// orthodox canon
	Response();
	Response(const Response &copy);
	Response &operator=(const Response &assign);
	~Response();
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
	std::string generateHttpResponse(std::string contentType);

	// methods
};

#endif
