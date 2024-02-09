/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 12:10:40 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/09 19:20:57 by jsarabia         ###   ########.fr       */
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
	unsigned int				port; // listen
	std::string					name; // server_name
	std::string					root; // root
	std::string					index; // index
	std::string					host; // host
	std::list<std::string>		actions; //actions all
	std::map<int, std::string>	errorPages; // error_page
	unsigned long				maxClientBodySize; // client_max_body_size
	//std::vector<Location>		locations; // location


public:
	// orthodox canonical form

	Server();
	Server(const Server &copy);
	Server &operator=(const Server &assign);
	~Server();
	// setters and getters

	void						setPort(std::string str);
	void						setName(std::string str);
	void						setActions(std::string str);
	void						setRoot(std::string str);
	int							getPort(void);
	std::string					getRoot(void);
	std::string					getName(void);
	std::list<std::string> 		getActions(void);
	// methods

	void						emptyActions(void);
};

#endif
