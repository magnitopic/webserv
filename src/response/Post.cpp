/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 18:18:14 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/19 16:01:54 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

void	handlePost(Location &location, Server &server, Request &req, Response &response)
{
	(void)location;
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
	if (!strncmp("multipart/form-data", post.getPostType().c_str(), 19)){
		post.setFileName(req.getReqBuffer());
		post.setFileContent(req.getReqBuffer());
		ofstream newfile(req.getAbsPath() + "/" + post.getFileName());
		newfile << post.getFileContent();
		newfile.close();
		response.setErrorCode(201);
		response.generateResponse(201, response.getErrorMsg(201), server);
		response.setContentLength(response.getResponse());
		response.generateHeader(201, server);
		return;
	}
	else if (!strncmp("text/plain", post.getPostType().c_str(), 10)){
		// TODO: Fill with code
	}
	else if (!strncmp("multipart/form-data", post.getPostType().c_str(), 19)){
		// TODO: Fill with code
	}
	else{ // This should be treated by default as an application/octet-stream
		// TODO: Fill with code
	}
	cout << req.getReqBuffer() << endl;
}
