/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 18:49:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/19 19:23:47 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"
#include "../../include/Socket.hpp"

Socket::Socket(unsigned int port)
{
	this->port = port;

	// Create socket
	this->socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socketFD == -1)
		raiseError("Error creating socket");

	// Non-blocking
	if (fcntl(this->socketFD, F_SETFL, fcntl(this->socketFD, F_GETFL, 0) | O_NONBLOCK) < 0)
		raiseError("Setting socket as non-blocking");

	// Reset socket to reuse address
	int reuseAddr = 1;
	if (setsockopt(this->socketFD, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr)) == -1)
		raiseError("Error setting socket option");


	// Bind socket
	sockaddr_in serverScruct;
	serverScruct.sin_family = AF_INET;
	serverScruct.sin_addr.s_addr = INADDR_ANY;
	serverScruct.sin_port = htons(this->port);
	if (bind(this->socketFD, (struct sockaddr *)&serverScruct, sizeof(serverScruct)) == -1)
		raiseError("Error binding socket");

	// Listen socket
	if (listen(this->socketFD, 10) == -1)
		raiseError("Error listening socket");
}

Socket::Socket(const Socket &copy)
{
	*this = copy;
}

Socket &Socket::operator=(const Socket &assign)
{
	if (this != &assign)
	{
		this->socketFD = assign.socketFD;
		this->port = assign.port;
	}
return *this;
}

Socket::~Socket()
{
}

int Socket::getSocketFD(void)
{
	return this->socketFD;
}
