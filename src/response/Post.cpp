/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 18:18:14 by jsarabia          #+#    #+#             */
/*   Updated: 2024/03/01 14:32:49 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"


static void	handleMultipartFormData(PostReq& post, Request& req, Response& response, Server& server)
{
	if (post.getBoundary().find("WebKitFormBoundary") > post.getBoundary().length() || post.getBoundary().find("WebKitFormBoundary") < 0){
		post.setFileName(req.getReqBuffer());
		post.setFileContent(req.getReqBuffer());
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
	else{
		post.setFileName(req.getReqBuffer());
		post.setFileContent(req.getReqBuffer());
		if (access("/uploads", W_OK)){
			response.setErrorCode(403);
			response.generateResponse(403, response.getErrorMsg(403), server);
			response.setContentLength(response.getResponse());
			response.generateHeader(403, server);
		}
		else if (access("/uploads", F_OK)){
			response.setErrorCode(404);
			response.generateResponse(404, response.getErrorMsg(404), server);
			response.setContentLength(response.getResponse());
			response.generateHeader(404, server);
		}
		else{
			cgiForPostReq(post, req, response, server);
			response.setErrorCode(201);
			response.generateResponse(201, response.getErrorMsg(201), server);
			response.setContentLength(response.getResponse());
			response.generateHeader(201, server);
		}
	}
}

static void	handleTextPlain(PostReq& post, Request& req, Response& response, Server& server)
{
	post.setContentTextPlain(bodyReq(req.getReqBuffer()));
	cgiForPostReq(post, req, response, server);

}

void	handlePost(Server &server, Request &req, Response &response)
{
	PostReq	post;
	if (response.getErrorCode() > 1)
		return;
	int pos = req.getReqBuffer().find("Content-Type:") + 14;
	if (pos >= static_cast<int>(req.getReqBuffer().length()) || pos < 14){
		post.setPostType("");
		response.setErrorCode(200);
		response.setResponseHTML(200);
		response.setContentLength(response.getResponse());
		response.generateHeaderContent(201, "text/html", server);
		return;
	}
	std::istringstream temp(req.getReqBuffer().substr(pos, req.getReqBuffer().length() - pos));
	std::string num;
	std::getline(temp, num);
	num += "\n";
	post.setPostType(num);
	post.setBoundary(num);
	std::string absPath = req.getAbsPath();
	if (!strncmp("multipart/form-data", post.getPostType().c_str(), 19)){
		handleMultipartFormData(post, req, response, server);
		return;
	}
	else if (!strncmp("text/plain", post.getPostType().c_str(), 10)){
		handleTextPlain(post, req, response, server);
	}
	else if (!strncmp("application/x-www-form-urlencoded", post.getPostType().c_str(), 33)){
		const char* arr[] = {"python3", absPath.c_str()};
		execve(arr[0], const_cast<char **>(arr), NULL);
	}
	else{ // This should be treated by default as an application/octet-stream
		// TODO: Fill with code
	}
}
