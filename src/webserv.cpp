/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/22 11:23:09 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

/*
	steps for creating contections:

	-	Create socket
	-	Bind socket
	-	Listen socket
	-	Accept and recive data
	-	Disconnect
 */

void createConection()
{
	// Create socket
	int socketVal = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socketVal == -1)
		raiseError("error creating socket");

	// Reset socket to reuse address
	int reuseAddr = 1;
	if (setsockopt(socketVal, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr)) == -1)
	{
		close(socketVal);
		raiseError("Error setting socket option");
	}

	// Bind socket
	sockaddr_in serverScruct;
	serverScruct.sin_family = AF_INET;
	serverScruct.sin_addr.s_addr = INADDR_ANY;
	serverScruct.sin_port = htons(8080);

	if (bind(socketVal, (struct sockaddr *)&serverScruct, sizeof(serverScruct)) == -1)
	{
		close(socketVal);
		raiseError("error binding socket");
	}

	// Listen socket
	if (listen(socketVal, 10) == -1)
	{
		close(socketVal);
		raiseError("error socket listening");
	}

	// Accept and recive data
	std::vector<pollfd> clients;
	while (true)
	{
		// registering a new client
		sockaddr_in clientAddress;
		socklen_t clientAddrSize = sizeof(clientAddress);
		int clientSocket = accept(socketVal, (struct sockaddr *)&clientAddress, &clientAddrSize);
		if (clientSocket == -1)
			raiseError("error accepting connection");
		else
		{
			pollfd pollFd;
			pollFd.fd = clientSocket;
			pollFd.events = POLLIN;
			clients.push_back(pollFd);
		}

		// polling data from clients
		int pollVal = poll(clients.data(), clients.size(), -1);
		if (pollVal == -1)
			raiseError("error polling data");
		else if (pollVal > 0)
		{
			for (std::vector<pollfd>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if (it->revents == POLLIN)
				{
					char buffer[1024];
					int readVal = read(it->fd, buffer, 1024);
					if (readVal == -1)
						raiseError("error reading data");
					else if (readVal == 0)
					{
						close(it->fd);
						clients.erase(it);
					}
					else
					{
						std::cout << buffer << std::endl;
						std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>";
						int writeVal = write(it->fd, response.c_str(), response.length());
						if (writeVal == -1)
							raiseError("error writing data");
					}
				}
			}
		}
	}

	// Disconnect
	close(socketVal);
}

int main(int argc, char **argv)
{
	if (argc > 2)
		raiseError("Too many arguments");
	else if (argc == 2)
		parseConfigFile(argv[1]);
	else
		parseConfigFile("webserv.conf");
	while (42)
	{
		createConection();
		sleep(1);
	}
	return 0;
}
