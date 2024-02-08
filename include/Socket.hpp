/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:58:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/08 16:56:34 by alaparic         ###   ########.fr       */
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
	int	fd;
	int	socketFD;
	

public:
	// orthodox canonical form

	Socket(void);
	Socket(const Socket &socket);
	Socket &operator=(const Socket &socket);
	~Socket(void);
	// setters and getters

	
	// methods

	void	createSocket(Server &server);
	void	bindSocket(Server &server);
	void	listenSocket(Server &server);
	
};


#endif
