/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:58:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/22 18:01:13 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	int					listen_sd;
	int					rc;
	int					nfds;
	int					on;
	int					timeout;
	int					new_sd;
	struct sockaddr_in6	addr;


public:
	// orthodox canonical form

	Socket();
	Socket(unsigned int port);
	Socket(const Socket &socket);
	Socket &operator=(const Socket &socket);
	~Socket(void);
	// setters and getters

	int		getListen_sd();
	void	setRc(int num);
	int		getRc();
	int		getNfds();
	int		getTimeout();
	void	setTimeout(int time);
	void	setNew_sd(int num);
	int		getNew_sd();
	void	increaseNfds();
	void	decrementNfds();

	// methods

	void	createSocket();
	void	bindSocket(unsigned int port);
	void	listenSocket();
	void	justWaiting();

};


#endif
