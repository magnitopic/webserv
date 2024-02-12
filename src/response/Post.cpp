/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 18:18:14 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/12 12:48:04 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

void	handlePost(Location &location, Server &server, Request &req, Response &response)
{
	(void)location;
	PostReq	post;
	int pos = req.getReqBuffer().find("Content-Type:") + 14;
	if (pos >= static_cast<int>(req.getReqBuffer().length()) || pos < 14){
		post.setPostType("");
		response.setResponseHTML(200);
		response.setContentLength(response.getResponse());
		response.generateHeaderContent(200, "text/html", server);
		return;
	}
	std::istringstream temp(req.getReqBuffer().substr(pos, req.getReqBuffer().length() - pos));
	std::string num;
	std::getline(temp, num, '\n');
	post.setPostType(num);
	post.setBoundary(num);
	if (!strncmp("multipart/form-data;", post.getPostType().c_str(), 20)){
		post.setFileName(req.getReqBuffer());
		post.setFileContent(req.getReqBuffer());
		ofstream newfile(req.getAbsPath() + "/" + post.getFileName());
		newfile << post.getFileContent();
		newfile.close();
		response.generateResponse(200, response.getErrorMsg(200), server);
		response.setContentLength(response.getResponse());
		response.generateHeader(200, server);
		return;
	}
}
