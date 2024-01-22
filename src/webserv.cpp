/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/22 19:06:30 by jsarabia         ###   ########.fr       */
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

void createConection(std::string str)
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
	serverScruct.sin_port = htons(setPort(str));

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
		std::cout << "poll value:" << pollVal << std::endl;
		if (pollVal == -1)
			raiseError("error polling data");
		else if (pollVal > 0)
		{
			// iterate through the clients and remove connection if no read value
			std::vector<pollfd>::iterator it = clients.begin();
			std::cout << "here" << std::endl;
			while (it != clients.end())
			{
				if (it->revents == POLLIN)
				{
					char buffer[1024];
					int readVal = read(it->fd, buffer, 1024);
					if (readVal == -1)
					{
						raiseError("error reading data");
						it++;
					}
					else if (readVal == 0)
					{
						close(it->fd);
						it = clients.erase(it);
					}
					else
					{
						std::cout << buffer << std::endl;
						std::string response;
						e_action	action = setAction(buffer);
						std::string aux = buffer;
						std::string directory = aux.substr(aux.find("/"), aux.find(" HTTP") - aux.find(" ") - 1); // Now we should check if the action can be performed in the chosen directory, if not thwrow error ¿501?
						if (action == GET){
							if (directory.compare("/") == 0)
								response = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\n\n" +
										getFile("pages/index.html");
							else if (std::string(buffer).find("GET /info HTTP/1.1") != std::string::npos)
								response = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\n\n" +
										getFile("pages/info/geco.html");
							else
								response = "HTTP/1.1 404 Not Found\nContent-Type: text/html; charset=utf-8\n\n" +
										getFile("pages/error_404.html");
						}
						int writeVal = write(it->fd, response.c_str(), response.length());
						std::cout << response << std::endl;
						if (writeVal == -1)
							raiseError("error writing data");
						close(it->fd);
						it = clients.erase(it);
						// it++;
					}
				}
			}
		}
	}
}

int main(int argc, char **argv)
{
	std::string file;

	if (argc > 2)
		raiseError("Too many arguments");
	else if (argc == 2)
	{
		parseConfigFile(argv[1]);
		file = configToString(argv[1]);
	}
	else
	{
		char *temp = strdup("webserv.conf");
		parseConfigFile(temp);
		file = configToString(temp);
		free(temp);
	}
	while (42)
	{
		createConection(file);
		sleep(1);
	}
	return 0;
}
