/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostReq.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:22:53 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/09 20:02:17 by jsarabia         ###   ########.fr       */
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
	int pos = request.find("Content-Type:");
	if (pos >= static_cast<int>(request.length()) || pos < 0){
		this->content = "";
		return;
	}
	std::istringstream temp(request.substr(pos, request.length() - pos));
	std::string cont;
	std::getline(temp, cont, '\n');
	int i = request.find(cont);
	this->content = request.substr(i + cont.length(), request.length() - i - cont.length());
}
