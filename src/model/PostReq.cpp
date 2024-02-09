/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostReq.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:22:53 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/09 21:21:04 by jsarabia         ###   ########.fr       */
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
	std::string aux2 = postType;
	std::string	reqcpy = request;
	while (aux2 == postType){
		pos = reqcpy.find("Content-Type:") + 14;
		std::istringstream aux(reqcpy.substr(pos, reqcpy.length() - pos));
		std::getline(aux, aux2, '\n');
		reqcpy = reqcpy.substr(pos, reqcpy.length() - pos);
	}
	std::istringstream temp(reqcpy.substr(0, reqcpy.length()));
	std::string cont;
	std::getline(temp, cont, '\n');
	while ((cont.find(boundary) > cont.length() || cont.find(boundary) < 0) && cont.length() > 0){
		std::getline(temp, cont, '\n');
		this->content += cont + "\n";
	}
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
	std::istringstream aux(request.substr(pos, request.length() - pos));
	std::string aux2;
	std::getline(aux, aux2, '\n');
	this->boundary = aux2;
}

std::string	PostReq::getBoundary()
{
	return this->boundary;
}
