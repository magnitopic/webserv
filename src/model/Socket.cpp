/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 18:49:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/03/08 15:57:27 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"
#include "../../include/Socket.hpp"

Socket::Socket()
{
	this->on = 1;
	this->nfds = 1;
	this->new_sd = 1;
	return;
}

Socket::Socket(const Socket &copy)
{
	*this = copy;
}

Socket &Socket::operator=(const Socket &assign)
{
	if (this != &assign)
	{
		this->listen_sd = assign.listen_sd;
		this->addr = assign.addr;
	}
	return *this;
}

Socket::~Socket()
{
}

int Socket::getListen_sd()
{
	return this->listen_sd;
}

int Socket::getRc()
{
	return this->rc;
}

void Socket::setRc(int num)
{
	this->rc = num;
}

void Socket::setTimeout(int time)
{
	this->timeout = time;
}

int Socket::getNew_sd()
{
	return this->new_sd;
}

void Socket::increaseNfds()
{
	this->nfds++;
}

void Socket::decrementNfds()
{
	this->nfds--;
}

void Socket::setNew_sd(int num)
{
	this->new_sd = num;
}

int Socket::getTimeout()
{
	return this->timeout;
}

int Socket::getNfds()
{
	return this->nfds;
}

void Socket::createSocket()
{
	// Creation of the Socket

	this->listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
	if (this->listen_sd < 0)
		raiseError("socket() failed");

	// Allowing socket fd to be reusable

	this->rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&this->on), sizeof(this->on));
	if (this->rc < 0)
	{
		close(listen_sd);
		raiseError("setsockopt() failed");
	}

	// Setting socket as non-blocking

	this->rc = fcntl(this->listen_sd, F_SETFL, O_NONBLOCK);
	if (this->rc < 0)
	{
		close(listen_sd);
		raiseError("ioctl() failed");
	}
}

void Socket::bindSocket(unsigned int port)
{
	memset(&this->addr, 0, sizeof(this->addr));
	this->addr.sin6_family = AF_INET6;
	memcpy(&this->addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	this->addr.sin6_port = htons(port);
	//address.sin_addr.s_addr = inet_addr(ips[0].c_str()); // TODO: read the config file to read ips
	bind(this->listen_sd, reinterpret_cast<struct sockaddr *>(&this->addr), sizeof(this->addr));
}

void Socket::listenSocket()
{
	// The second argument of the listen() function is the number of connecgtions allowed in the incoming queue

	this->rc = listen(this->listen_sd, 20);
	if (this->rc < 0)
	{
		close(this->listen_sd);
		raiseError("listen() failed");
	}
}
