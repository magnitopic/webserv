/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 18:33:01 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/14 17:33:08 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

void	getMethod(Location &location, Server &server, Request &req, Response &resp)
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
	req.setAbsPath(server);
	req.setExtension();
	if (req.getAbsPath()[req.getAbsPath().length() - 1] == '/')
		req.handleSlash();
	if (stat(req.getAbsPath().c_str(), &s) == 0 && s.st_mode & S_IFREG)
	{
		resp.setErrorCode(200);
		resp.setResponse(getFile(req.getAbsPath()));
		resp.setContentLength(resp.getResponse());
		resp.generateHeader(200, server);
		req.setContentType(parseContentType(req.getExtension()));
		resp.generateHeaderContent(200, req.getContentType(), server);
	}
	else if (access(req.getAbsPath().c_str(), F_OK) == 0 &&
			 stat((server.getRoot() + location.getIndex()).c_str(), &s) == 0 && S_ISREG(s.st_mode))
	{
		resp.setErrorCode(200);
		resp.setResponse(getFile(server.getRoot() + location.getIndex()));
		resp.setContentLength(resp.getResponse());
		req.setContentType(parseContentType("html"));
		resp.generateHeaderContent(200, req.getContentType(), server);
	}
	else if (location.getAutoIndex() == true)
	{
		resp.setErrorCode(200);
		location.generateAutoIndex(server, location.getDirectory(), location, resp);
		resp.setContentLength(resp.getResponse());
		resp.generateHeader(200, server);
	}
	else
	{
		resp.setErrorCode(404);
		resp.setResponseNotFound();
		resp.setContentLength(resp.getResponse());
		req.setContentType(parseContentType("html"));
		resp.generateHeader(404, server);
	}
}

