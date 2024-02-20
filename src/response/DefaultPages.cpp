/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultPages.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 19:37:25 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/20 16:20:08 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

void	Response::setResponseNotFound()
{
	this->response = "<html><head><title>Error 404</title><link rel=\"shortcut icon\" href=\"../../../images/favicon.ico\" type=\"image/x-icon\"></head><body><h1>Ups, this page was absorbed by the Black Hole</h1>";
	this->response += "<img src=\"../../../images/marvin.png\" width='530' height='400'></img></body></html>";
}

void	Response::setResponseHTML(int code)
{
	this->response = "<html><head><title>" + to_string(code) + " " + this->getErrorMsg(code);
	this->response += "</title>></head><body><h1>" + this->getErrorMsg(code) + "</h1>";
	this->response += "</body></html>";
}

void	Response::setResponseForbidden()
{
	this->response = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\" />\n<title>403 Forbidden</title>\n</head>\n<body>\n<center>\n<h1>Error 403 - Forbidden</h1>\n<hr width=\"80%\" />\n</center>\n</body>\n</html>\n";
}

void	continueConnecting()
{
	std::string response;
	response = "HTTP/1.1 ";
	response += "100";
	response += " Continue";
	response += "\n\n";
	//send()
}
