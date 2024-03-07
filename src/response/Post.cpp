/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 18:18:14 by jsarabia          #+#    #+#             */
/*   Updated: 2024/03/07 15:56:48 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../include/webserv.hpp"

static void handleMultipartFormData(PostReq &post, Request &req, Response &response, Server &server)
{
	post.setFileName(req.getReqBuffer());
	post.setFileContent(req.getReqBuffer());
	if (post.getFileName().length() < 1)
	{
		response.setErrorCode(400);
		response.generateResponse(400, response.getErrorMsg(400), server);
		response.setContentLength(response.getResponse());
		response.generateHeader(400, server);
		return;
	}
	if (post.getBoundary().find("WebKitFormBoundary") > post.getBoundary().length() || post.getBoundary().find("WebKitFormBoundary") < 0)
	{
		std::string name = req.getAbsPath() + "/" + post.getFileName();
		ofstream newfile(name);
		open(name.c_str(), O_RDWR | O_CREAT, 0666);
		newfile << post.getFileContent();
		newfile.close();
		response.setErrorCode(201);
		response.generateResponse(201, response.getErrorMsg(201), server);
		response.setContentLength(response.getResponse());
		response.generateHeader(201, server);
		return;
	}
	else
	{
		post.setFileName(req.getReqBuffer());
		post.setFileContent(req.getReqBuffer());
		std::string up = server.getTheRoot() + "/uploads";
		if (access(up.c_str(), W_OK))
		{
			response.setErrorCode(403);
			response.generateResponse(403, response.getErrorMsg(403), server);
			response.setContentLength(response.getResponse());
			response.generateHeader(403, server);
		}
		else if (access(up.c_str(), F_OK))
		{
			response.setErrorCode(404);
			response.generateResponse(404, response.getErrorMsg(404), server);
			response.setContentLength(response.getResponse());
			response.generateHeader(404, server);
		}
		else
		{
			cgiForPostReq(post, req, response, server);
			if (response.getErrorCode() < 90)
			{
				response.setErrorCode(201);
				response.generateResponse(201, response.getErrorMsg(201), server);
				response.setContentLength(response.getResponse());
				response.generateHeader(201, server);
			}
		}
	}
}

static void handleTextPlain(PostReq &post, Request &req, Response &response, Server &server)
{
	post.setContentTextPlain(bodyReq(req.getReqBuffer()));
	cgiForPostReq(post, req, response, server);
}

static void handleApplication(PostReq &post, Request &req, Response &response, Server &server)
{
	post.setContentTextPlain(bodyReq(req.getReqBuffer()));
	cgiForPostReq(post, req, response, server);
}

void handlePost(Server &server, Request &req, Response &response)
{
	PostReq post;
	if (response.getErrorCode() > 1)
		return;
	int pos = req.getReqBuffer().find("Content-Type:") + 14;
	if (pos >= static_cast<int>(req.getReqBuffer().length()) || pos < 14)
	{
		post.setPostType("");
		response.setErrorCode(200);
		response.setResponseHTML(200);
		response.setContentLength(response.getResponse());
		response.generateHeaderContent(200, "text/html", server);
		return;
	}
	std::istringstream temp(req.getReqBuffer().substr(pos, req.getReqBuffer().length() - pos));
	std::string num;
	std::getline(temp, num);
	num += "\n";
	post.setPostType(num);
	post.setBoundary(num);
	std::string absPath = req.getAbsPath();
	if (!strncmp("multipart/form-data", post.getPostType().c_str(), 19))
	{
		handleMultipartFormData(post, req, response, server);
		return;
	}
	else if (!strncmp("text/plain", post.getPostType().c_str(), 10))
	{
		handleTextPlain(post, req, response, server);
	}
	else if (!strncmp("application/x-www-form-urlencoded", post.getPostType().c_str(), 33))
	{
		handleApplication(post, req, response, server);
	}
	else
	{ // This should be treated by default as an application/octet-stream
		handleTextPlain(post, req, response, server);
		return;
	}
}
