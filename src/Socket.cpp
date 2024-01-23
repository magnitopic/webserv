/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 18:49:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/01/23 17:40:52 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Socket.hpp"
#include "../include/webserv.hpp"

Socket::Socket(void){
	return;
}

Socket::Socket(std::string directory): directory(directory){
	return;
}

Socket::Socket(const Socket &socket){
	this->directory = socket.directory;
	this->actions = socket.actions;
	this->response = socket.response;
	return;
}

Socket& Socket::operator=(const Socket &socket){
	this->directory = socket.directory;
	this->actions = socket.actions;
	this->response = socket.response;
	return *this;
}

Socket::~Socket(void){
	return;
}

void	Socket::setDirectory(std::string directory){
	this->directory = directory;
	return;
}

std::string	Socket::getDirectory(void){
	return this->directory;
}

void	Socket::setActions(std::string directory, std::string text){
	std::string aux = "location " + directory;
	if (text.find(aux) >= text.length())
		return;
	std::string methods = text.substr(text.find(aux), text.find("}") - text.find(aux));
	std::string word;
	int i = methods.find("limit_except") + 12;
	while (i < static_cast<int>(methods.length())){
		if (isupper(methods[i]))
			word.push_back(methods[i]);
		else if (islower(methods[i]))
			break;
		else{
			if (word.length() > 0)
				this->actions.push_back(word);
			word = "";
		}
		i++;
	}
}

std::ostream &operator<<(std::ostream &os, std::list<std::string> &list){
	for (std::list<std::string>::iterator it = list.begin(); it != list.end(); it++)
		os << *it << " ";
	return os;
}
