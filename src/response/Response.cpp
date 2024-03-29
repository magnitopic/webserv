/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 18:21:49 by jsarabia          #+#    #+#             */
/*   Updated: 2024/03/06 16:35:41 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

Response::Response()
{
	this->errorCode.insert(std::pair<int, std::string>(100, "Continue"));
	this->errorCode.insert(std::pair<int, std::string>(200, "OK"));
	this->errorCode.insert(std::pair<int, std::string>(201, "Created"));
	this->errorCode.insert(std::pair<int, std::string>(202, "Accepted"));
	this->errorCode.insert(std::pair<int, std::string>(204, "No Content"));
	this->errorCode.insert(std::pair<int, std::string>(301, "Moved Permanently"));
	this->errorCode.insert(std::pair<int, std::string>(302, "Moved Temporarily"));
	this->errorCode.insert(std::pair<int, std::string>(400, "Bad Request"));
	this->errorCode.insert(std::pair<int, std::string>(401, "Unauthorized"));
	this->errorCode.insert(std::pair<int, std::string>(403, "Forbidden"));
	this->errorCode.insert(std::pair<int, std::string>(404, "Not Found"));
	this->errorCode.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
	this->errorCode.insert(std::pair<int, std::string>(413, "Request Entity Too Large"));
	this->errorCode.insert(std::pair<int, std::string>(418, "I'm a teapot"));
	this->errorCode.insert(std::pair<int, std::string>(500, "Internal Server Error"));
	this->errorCode.insert(std::pair<int, std::string>(501, "No Implemented"));
	this->errorCode.insert(std::pair<int, std::string>(502, "Bad Gateway"));
	this->errorCode.insert(std::pair<int, std::string>(503, "Service Unavailable"));
	this->errorCode.insert(std::pair<int, std::string>(504, "Gateway Timeout"));
	this->errorCode.insert(std::pair<int, std::string>(508, "Loop Detected"));
	this->code = 0;
}

Response::Response(const Response &copy)
{
	this->errorCode = copy.errorCode;
}

Response &Response::operator=(const Response &assign)
{
	this->errorCode = assign.errorCode;
	return *this;
}

Response::~Response()
{
	return;
}

std::string Response::getErrorMsg(int id)
{
	return this->errorCode.find(id)->second;
}

void Response::generateResponse(int code, std::string def, Server &server)
{
	if (server.getErrorPages().size() > 0)
	{
		std::map<int, std::string>::const_iterator it = getMapIterator(server.getErrorPages(), code);
		if (it != server.getErrorPages().end())
		{
			if (server.codeInErrorPages(code))
				this->response = getFile(server.getTheRoot() + "/" + server.getErrorPages().find(code)->second);
			if (this->response.length() > 0)
				return;
		}
	}
	this->response = "<html>\n<head>\n<title>";
	this->response += to_string(code);
	this->response += " " + def;
	this->response += "</title>\n</head>\n<body>\n<center><h1>";
	this->response += to_string(code);
	this->response += " " + def;
	this->response += "</h1></center>\n<hr><center>";
	this->response += server.getName();
	this->response += "</center>\n</body>\n</html>";
}

std::string Response::getResponse(void)
{
	return this->response;
}

void Response::setResponse(std::string response)
{
	this->response = response;
	return;
}

void Response::setContentLength(std::string cont)
{
	int size = static_cast<int>(cont.length());
	this->contentLength = size;
}

int Response::getContentLength(void)
{
	return this->contentLength;
}

void Response::generateHeader(int code, Server &server)
{
	this->header = "HTTP/1.1 ";
	this->header += to_string(code);
	this->header += " " + getErrorMsg(code);
	this->header += "\nServer: " + server.getName();
	this->header += "\nContent-Type: text/html; charset=utf-8\n";
	this->header += "Content-Length: ";
	this->header += std::to_string(this->contentLength);
	this->header += "\n\n";
	return;
}

void Response::generateHeaderContinue(int code)
{
	this->header = "HTTP/1.1 ";
	this->header += to_string(code);
	this->header += " " + getErrorMsg(code);
	this->header += "\r\n\r\n";
	return;
}

void Response::generateHeaderContent(int code, std::string type, Server &server)
{
	this->header = "HTTP/1.1 ";
	this->header += to_string(code);
	this->header += " " + getErrorMsg(code);
	this->header += "\nServer: " + server.getName();
	this->header += "\nContent-Type: " + type;
	this->header += "; charset=utf-8\n";
	this->header += "Content-Length: ";
	this->header += std::to_string(this->contentLength);
	this->header += "\n\n";
	return;
}

void Response::generateTeapotResponse(int code, Server &server)
{
	if (server.getErrorPages().size() > 0)
	{
		if (server.codeInErrorPages(code))
		{
			this->response = getFile(server.getTheRoot() + "/" + server.getErrorPages().find(code)->second);
			if (this->response.length() > 0)
				return;
		}
	}
	this->response = "<html>\n<head>\n<title>";
	this->response += to_string(code);
	this->response += " " + getErrorMsg(code);
	this->response += "</title>\n<link rel=\"shortcut icon\" href=\"images/favicon.ico\" type=\"image/x-icon\"></head>\n<body>\n<center><img src=\"images/our_server.jpg\"></img></center><center><h1>";
	this->response += to_string(code);
	this->response += " " + getErrorMsg(code);
	this->response += "</h1></center>\n<hr><center>";
	this->response += server.getName();
	this->response += "</center>\n</body>\n</html>";
}

std::string Response::getHeader(void)
{
	return this->header;
}

std::string Response::generateHttpResponse()
{
	std::string resp = "";
	resp += this->header;
	resp += this->response;
	return resp;
}

void Response::setErrorCode(int code)
{
	this->code = code;
}

int Response::getErrorCode()
{
	return this->code;
}

void showData(Request &req, Response &response)
{
	std::time_t currentTime = std::time(nullptr);

	// Convert to local time structure
	std::tm *localTime = std::localtime(&currentTime);

	// Access components of the time structure
	int year = localTime->tm_year + 1900;
	int month = localTime->tm_mon + 1; // Note: Months are zero-based
	int day = localTime->tm_mday;
	int hour = localTime->tm_hour;
	int minute = localTime->tm_min;
	int second = localTime->tm_sec;

	std::string uri = req.getOriginalUri();
	if (uri.find("?") >= 0 && uri.find("?") < uri.length())
		uri = uri.substr(0, uri.find("?"));

	// Print the result
	std::cout << "[" << day << "-" << month << "-" << year << " " << hour << ":" << minute << ":" << second << "] \"" << req.getMethod() << " " << uri << " HTTP/1.1\" " << response.getErrorCode() << " " << response.getContentLength() << std::endl;
}

void Response::generateRedirection(Location &location)
{
	this->response = "Redirecting to " + location.getRedirection().begin()->second;
}

void Response::generateRedirectHeader(Location &location, Server &server)
{
	this->header = "HTTP/1.1 ";
	this->header += to_string(location.getRedirection().begin()->first);
	this->header += " " + getErrorMsg(location.getRedirection().begin()->first);
	this->header += "\nServer: " + server.getName();
	this->header += "\nContent-Type: text/html; charset=utf-8\n";
	this->header += "Location: ";
	this->header += location.getRedirection().begin()->second;
	this->header += "\n";
	this->header += "Content-Length: ";
	this->header += std::to_string(this->contentLength);
	this->header += "\n\n";
	this->header += this->response;
}
