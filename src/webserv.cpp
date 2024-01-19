/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/19 11:26:13 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	int socketVal = socket(AF_INET, SOCK_STREAM, 0);

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

	// Accept and revice data
	sockaddr_in clientAddress;
	socklen_t clientAddrSize = sizeof(clientAddress);
	int clientSocket = accept(socketVal, (struct sockaddr *)&clientAddress, &clientAddrSize);
	if (clientSocket == -1)
	{
		close(socketVal);
		raiseError("error accepting connection");
	}

	char buffer[1024];
	int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesRead == -1)
	{
		perror("error receiving data");
	}
	else
	{
		buffer[bytesRead] = '\0';
		std::cout << "Received data: " << buffer << std::endl;
	}

	// Disconnect
	close(clientSocket);
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
