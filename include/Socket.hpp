/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:58:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/21 14:48:10 by jsarabia         ###   ########.fr       */
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
	int					socketFD;
	int					listen_sd;
	int					rc;
	int					on;
	int					nfds;
	int					timeout;
	int					new_sd;
	struct sockaddr_in6	addr;
	struct pollfd		fds[200];


public:
	// orthodox canonical form

	Socket();
	Socket(unsigned int port);
	Socket(const Socket &socket);
	Socket &operator=(const Socket &socket);
	~Socket(void);
	// setters and getters

	int getSocketFD(void);
	// methods

	void	createSocket();
	void	bindSocket(std::vector<Server> &server);
	void	listenSocket();
	void	initializePollfdStruct();
	void	justWaiting();

};


#endif
