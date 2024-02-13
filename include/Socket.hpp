/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:58:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/12 18:02:02 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <map>
#include <list>
#include <string>
#include <algorithm>
#include "webserv.hpp"

class Server;
/**
 * 
*/
class Socket
{
private:
	int	socketFD;
	unsigned int	port;
	

public:
	// orthodox canonical form

	Socket(unsigned int port);
	Socket(const Socket &socket);
	Socket &operator=(const Socket &socket);
	~Socket(void);
	// setters and getters

	int getSocketFD(void);
	// methods

	void	createSocket(Server &server);
	void	bindSocket(Server &server);
	void	listenSocket(Server &server);
	
};


#endif
