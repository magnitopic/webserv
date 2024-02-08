/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:58:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/08 07:52:32 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <map>
#include <list>
#include <string>
#include <algorithm>

class Response;

/*

The aim of this class is to store the type of action requested by the socket
(GET, POST, DELETE), the name of the directory in which to perform that action
(e.g.: GET /), and the actions that are allowed and denied inside this directory
in the configuration file in order to check wether it is possible or not to
fulfill the request.

*/

class Server;

class Socket
{
private:
	std::string				directory;
	std::list<std::string>	actions; 		// actions that can be performed inside the directory
	std::list<std::string>	forbidden;		// actions denied in the directory
	std::string				header;			// header given to the request
	std::string				contentType;	// type of content that will be given to the request
	std::list<std::string>	actionsArr;		// this will be a list which contents will be ["GET", "POST", "DELETE"] and will be used to compare with the enum
	std::string				root;			// indicates the root directory for the location
	std::string				contentLength;
	bool					autoIndex;
public:
	// orthodox canonical form

	Socket(void);
	Socket(std::string directory);
	Socket(const Socket &socket);
	Socket &operator=(const Socket &socket);
	~Socket(void);
	// setters and getters

	void					setDirectory(std::string directory);
	std::string				getDirectory(void);
	void					setResponse(std::string response);
	std::string				getResponse(void);
	void					setForbidden(std::string directory, std::string text);
	void					setAutoIndex(bool autoIndex);
	bool					getAutoIndex(void);
	void					setActions(Server& server, std::string directory, std::string text);
	std::list<std::string>	getActions(void);
	std::string				getRoot(void);
	void					setContentType(std::string type);
	std::string				getContentType(void);
	std::list<std::string>	getForbidden(void);
	void					emptyActions(void);
	// methods

	std::string	generateHttpResponse(void);
	void		generateAutoIndex(Server& server, std::string route, Socket &socketClass, Response &response);
};

bool		isAutoindex(std::string str, Socket socketClass);
std::ostream &operator<<(std::ostream &os, std::list<std::string> list);  //instead of creating a printList function y created this operator overload function that works the same way

#endif
