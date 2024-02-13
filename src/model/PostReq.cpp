/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostReq.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:22:53 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/13 19:32:50 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

// orthodox canonical form

PostReq::PostReq() {}

PostReq::PostReq(const PostReq &copy)
{
	*this = copy;
}

PostReq &PostReq::operator=(const PostReq &assign)
{
	if (this != &assign)
	{
		this->fileName= assign.fileName;
	}
	return *this;
}

PostReq::~PostReq() {}

// setters and getters


void	PostReq::setFileName(std::string request)
{
	int pos = request.find("filename=\"") + 10;
	if (pos >= static_cast<int>(request.length()) || pos < 10){
		this->fileName = "";
		return;
	}
	std::string temp = request.substr(pos, request.length() - pos);
	this->fileName = temp.substr(0, temp.find("\""));
}

std::string	PostReq::getFileName()
{
	return this->fileName;
}

void	PostReq::setFileContent(std::string request)
{
	int pos = request.find("Content-Type:") + 14;
	if (pos >= static_cast<int>(request.length()) || pos < 14){
		this->content = "";
		return;
	}
	std::string	reqcpy = request.substr(pos, request.length() - pos);
	pos = reqcpy.find("Content-Type:") + 14;
	if (pos >= static_cast<int>(reqcpy.length()) || pos < 14){
		this->content = "";
		return;
	}
	reqcpy = reqcpy.substr(pos, request.length() - pos);
	reqcpy = reqcpy.substr(reqcpy.find("\n"), reqcpy.length() - reqcpy.find("\n"));
	if (boundary[boundary.length() - 1] == '\n' || boundary[boundary.length() - 1] == '\r')
		boundary = boundary.substr(0, boundary.length() - 1);
	std::size_t	pos2 = 0;
	std::size_t	check = reqcpy.rfind(boundary);
	if (check < reqcpy.length() && check >= 0)
	{
		while (pos2 < reqcpy.length())
		{
			pos2 = reqcpy.find(boundary);
			if (pos2 >= reqcpy.length() || pos2 < 0)
				break;
			reqcpy.erase(pos2, boundary.length());
		}
	}
	pos2 = 0;
	check = reqcpy.find(this->postType);
	if (check < reqcpy.length() && check >= 0)
	{
		while (pos2 < reqcpy.length())
		{
			pos2 = reqcpy.find(this->postType);
			if (pos2 >= reqcpy.length())
				break;
			reqcpy.erase(pos2, this->postType.length());
		}
	}
	this->content = reqcpy;
}

std::string	PostReq::getFileContent()
{
	return this->content;
}

void	PostReq::setPostType(std::string type){
	this->postType = type;
}

std::string	PostReq::getPostType()
{
	return this->postType;
}

void	PostReq::setBoundary(std::string request)
{
	int pos = request.find("boundary=") + 9;
	if (pos >= static_cast<int>(request.length()) || pos < 9){
		this->boundary = "";
		return;
	}
	size_t d = request.find("\n");
	this->boundary = request.substr(pos, d - pos);
}

std::string	PostReq::getBoundary()
{
	return this->boundary;
}
