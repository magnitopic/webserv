/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 18:49:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/21 18:22:49 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"
#include "../../include/Socket.hpp"

/*Socket::Socket(unsigned int port)
{
	this->port = port;

	// Create socket
	this->socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socketFD == -1)
		raiseError("Error creating socket");

	// Non-blocking
	int flag = fcntl(this->socketFD, F_GETFL, 0);
	if (fcntl(this->socketFD, F_SETFL, flag & ~O_NONBLOCK) < 0)
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
}*/

Socket::Socket(){
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
		this->socketFD = assign.socketFD;
		//this->port = assign.port;
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

void	Socket::createSocket()
{
	// Creation of the Socket

	this->listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
	if (this->listen_sd < 0)
		raiseError("socket() failed");

	// Allowing socket fd to be reusable

	this->rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&this->on), sizeof(this->on));
	if (this->rc < 0){
		close(listen_sd);
		raiseError("setsockopt() failed");
	}

	// Setting socket as non-blocking

	this->rc = ioctl(listen_sd, FIONBIO, (char *)&on);
	if (this->rc < 0){
		close(listen_sd);
		raiseError("ioctl() failed");
	}
}

void	Socket::bindSocket(std::vector<Server> &server)
{
	(void)server;
	memset(&this->addr, 0, sizeof(this->addr));
	this->addr.sin6_family = AF_INET6;
	memcpy(&this->addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	this->addr.sin6_port = htons(8080); // TODO: the port number is temporary until we can parse correctly the config file
	bind(this->listen_sd, reinterpret_cast<struct sockaddr *>(&this->addr), sizeof(this->addr));
}

void	Socket::listenSocket()
{
	// The second argument of the listen() function is the number of connecgtions allowed in the incoming queue

	this->rc = listen(this->listen_sd, 20);
	if (this->rc < 0){
		close(this->listen_sd);
		raiseError("listen() failed");
	}
}

void	Socket::initializePollfdStruct()
{
	memset(this->fds, 0, sizeof(fds));
	this->fds[0].fd = this->listen_sd;
	fds[0].events = POLLIN;

	// If no activity happended after 3 minutes the program will end

	this->timeout = (3 * 60 * 1000);
}

static void	closeConnections(struct pollfd fds[200], int nfds)
{
	for (int i = 0; i < nfds; i++){
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}
}

void	Socket::justWaiting()
{
	bool		end_server = false;
	bool		close_conn = false;
	bool		compress_array = false;
	char		buffer[800];
	std::string	finalBuf;

	memset(buffer, 0, sizeof(buffer));
	while (end_server == false)
	{
		cout << "Waiting on poll()..." << endl;
		this->rc = poll(this->fds, this->nfds, this->timeout);
		if (this->rc < 0){
			perror("poll() failed");
			break;
		}
		else if (this->rc == 0){
			cout << "Timeout. End program" << endl;
			break;
		}
		for (int i = 0; i < this->nfds; i++){
			if (this->fds[i].revents == 0)
				continue;
			if (this->fds[i].revents != POLLIN){
				cerr << "Error! revents = " << this->fds[i].revents << endl;
				end_server = true;
				break;
			}

			if (this->fds[i].revents == POLLOUT){
				cout << "HOLI" << endl;
				exit(0);
				break;
			}

			if (this->fds[i].fd == this->listen_sd)
			{
				cout << "Listening socket is readable" << endl;

				// Accept all queued incoming connections

				while (this->new_sd != -1){
					this->new_sd = accept(this->listen_sd, NULL, NULL);
					if (this->new_sd < 0){
						if (errno != EWOULDBLOCK){
							perror("accept() failed");
							end_server = true;
						}
						break;
					}

				// Adding incoming connection to the pollfd structure

					cout << "New incoming connection" << endl;
					this->fds[nfds].fd = this->new_sd;
					this->fds[nfds].events = POLLIN;
					this->nfds++;
				}
			}
			else{
				cout << "Descriptor " << fds[i].fd << " is readable" << endl;
				close_conn = false;
				this->rc = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (this->rc < 0){
					if (errno != EWOULDBLOCK){
						perror("recv() failed");
						close_conn = true;
					}
				}
				finalBuf += buffer;
				cout << "|" << finalBuf << "|" << endl;
				cout << parsedContentLength(finalBuf) << endl;
				if (this->rc == 0 || (static_cast<int>(bodyReq(finalBuf).length()) >= parsedContentLength(finalBuf) && parsedContentLength(finalBuf) > 0)){
					cout << "Connection closed" << endl;
					close_conn = true;
				}
				if (this->rc == 0){
					perror("send() failed");
					close_conn = true;
				}
				memset(buffer, 0, sizeof(buffer));
				//exit(0);
				//handleRequests(fds[i].fd, server, clients, str);
				if (close_conn)
				{
					this->rc = send(fds[i].fd, finalBuf.c_str(), finalBuf.size(), 0);
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_array = true;
				}
			}
		}
		if (compress_array){
			compress_array = false;
			for (int i = 0; i < nfds; i++){
				if (fds[i].fd == -1){
					for (int j = i; j < nfds; j++)
						fds[j].fd = fds[j + 1].fd;
					i--;
					nfds--;
				}
			}
		}
	}
	closeConnections(fds, nfds);
}
