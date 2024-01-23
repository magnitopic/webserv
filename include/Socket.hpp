/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:58:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/01/23 17:51:04 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <map>
#include <list>
#include <string>

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
		std::list<std::string>	actions; 	//actions that can be performed inside the directory
		std::list<std::string>	forbidden;	//actions denied in the directory
		std::string				response;	//response given to the request
	public:
		Socket(void);
		Socket(std::string directory);
		Socket(const Socket &socket);
		Socket &operator=(const Socket &socket);
		~Socket(void);
		void		setDirectory(std::string directory);
		std::string	getDirectory(void);
		void		setActions(std::string directory, std::string text);
		void		setForbidden(std::string directory, std::string text);
};

std::ostream &operator<<(std::ostream &os, std::list<std::string> &list);

#endif
