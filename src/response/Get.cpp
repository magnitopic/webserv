/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 18:33:01 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/06 18:34:47 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

void getMethod(Location &location, Server &server, Request &req, Response &resp)
{
	location.setAutoIndex(isAutoindex(location));
	/*
		The options for a response are:
		- if the petition is a file and it exists, return the file
		- if the petition is a directory and it exists, check if there is a default file (like index.html)
		- if the petition is a directory and it exists, render the auto-index page
		- if the petition does not exist, return a 404 error
	 */
	struct stat s;
	if (req.getAbsPath().substr(req.getAbsPath().length() - 3, 3) == ".py")
		cgiForGetReq(req, resp, server);
	else if (req.getUri() == "/teapot")
	{
		resp.setErrorCode(418);
		resp.generateTeapotResponse(418, server);
		resp.setContentLength(resp.getResponse());
		resp.generateHeader(418, server);
	}
	else if (stat(req.getAbsPath().c_str(), &s) == 0 && s.st_mode & S_IFREG) // This is a file
	{
		if (access(req.getAbsPath().c_str(), R_OK) != 0)
		{
			resp.setErrorCode(403);
			resp.generateResponse(403, resp.getErrorMsg(403), server);
			resp.setContentLength(resp.getResponse());
			resp.generateHeader(403, server);
		}
		else
		{
			resp.setErrorCode(200);
			resp.setResponse(getFile(req.getAbsPath()));
			resp.setContentLength(resp.getResponse());
			resp.generateHeader(200, server);
			req.setContentType(parseContentType(req.getExtension()));
			resp.generateHeaderContent(200, req.getContentType(), server);
		}
	}
	else if (access(req.getAbsPath().c_str(), F_OK) == 0 &&
			 stat((req.getAbsPath() + "/" + location.getIndex()).c_str(), &s) == 0 && S_ISREG(s.st_mode))
	{
		resp.setErrorCode(200);
		resp.setResponse(getFile(req.getAbsPath() + "/" + location.getIndex()));
		resp.setContentLength(resp.getResponse());
		req.setContentType(parseContentType("html"));
		resp.generateHeaderContent(200, req.getContentType(), server);
	}
	else if (location.getAutoIndex() == true && access(req.getAbsPath().c_str(), F_OK) == 0)
	{
		resp.setErrorCode(200);
		location.generateAutoIndex(server, location.getDirectory(), location, resp);
		resp.setContentLength(resp.getResponse());
		resp.generateHeader(200, server);
	}
	else
	{
		resp.setErrorCode(404);
		resp.generateResponse(404, resp.getErrorMsg(404), server);
		resp.setContentLength(resp.getResponse());
		req.setContentType(parseContentType("html"));
		resp.generateHeader(404, server);
	}
}
