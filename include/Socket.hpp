/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:58:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/01/31 16:05:59 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <map>
#include <list>
#include <string>
#include <algorithm>

/*

The aim of this class is to store the type of action requested by the socket
(GET, POST, DELETE), the name of the directory in which to perform that action
(e.g.: GET /), and the actions that are allowed and denied inside this directory
in the configuration file in order to check wether it is possible or not to
fulfill the request.

*/

class Socket
{
	private:
		std::string				directory;
		std::list<std::string>	actions; 	// actions that can be performed inside the directory
		std::list<std::string>	forbidden;	// actions denied in the directory
		std::string				header;		// header given to the request
		std::string				response;	// response given to the request
		std::list<std::string>	actionsArr;	// this will be a list which contents will be ["GET", "POST", "DELETE"] and will be used to compare with the enum
		std::string				root;		// indicates the root directory for the location
		std::string				contentLength;
		bool					autoIndex;
	public:
		Socket(void);
		Socket(std::string directory);
		Socket(const Socket &socket);
		Socket &operator=(const Socket &socket);
		~Socket(void);
		// setters & getters

		void					setDirectory(std::string directory);
		void					setResponse(std::string response);
		void					setHeader(std::string header);
		void					setContentLength(std::string cont);
		void					setActions(std::string directory, std::string text);
		void					setForbidden(std::string directory, std::string text);
		void					setAutoIndex(bool autoIndex);
		std::list<std::string>	getActions(void);
		std::string				getContentLength(void);
		std::string				getDirectory(void);
		std::string				getHeader(void);
		std::string				getResponse(void);
		std::string				&getActionsArray(int i);
		std::string				getRoot(void);
		bool					getAutoIndex(void);

		// methods
		void generateAutoIndex(std::string route, Socket &socketClass);
		void servePages();
};

std::ostream &operator<<(std::ostream &os, std::list<std::string> &list);  //instead of creating a printList function y created this operator overload function that works the same way

#endif
