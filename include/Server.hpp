/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 12:10:40 by jsarabia          #+#    #+#             */
/*   Updated: 2024/03/06 16:09:27 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "webserv.hpp"

/**
 * The aim of this class is to store the information provided by the server
 * directive of the configuration file provides. This information will be used
 * to create the server and the socket, and to check what type of responses will
 * be given.
*/
class Server
{
private:
	std::vector<Socket>			sockets;
	std::vector<unsigned int>	ports; // listen
	std::string					name; // server_name
	std::string					root; // root
	std::string					index; // index
	std::string					host; // host
	std::list<std::string>		actions; //actions all
	std::map<int, std::string>	errorPages; // error_page
	unsigned long				maxClientBodySize; // client_max_body_size
	std::vector<Location>		locations;	// vector of Location objects


public:
	// orthodox canonical form

	Server();
	Server(std::string str);
	Server(const Server &copy);
	Server &operator=(const Server &assign);
	~Server();
	// setters and getters

	void						setPort(std::string str);
	void						setName(std::string str);
	void						setActions(std::string str);
	void						setRoot(std::string str);
	void						setLocations(std::vector<Location> &locations);
	std::vector<Location>		getLocations();
	std::vector<unsigned int>	getPorts(void);
	std::string					getTheRoot(void);
	std::string					getName(void);
	std::list<std::string>		getActions(void);
	void						setMaxClientSize(std::string str);
	unsigned long				getMaxClientSize();
	void						setErrorPages(std::string str);
	std::map<int, std::string>	getErrorPages();
	// methods

	void						emptyActions(void);
	void						addSocket(const Socket &socket);
	Server						findServer(int fd);
	int							codeInErrorPages(int code);
};

#endif
