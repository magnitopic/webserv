/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/22 11:56:12 by alaparic         ###   ########.fr       */
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
		std::cout << "poll value:" << pollVal << std::endl;
		if (pollVal == -1)
			raiseError("error polling data");
		else if (pollVal > 0)
		{
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
						std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n \
						<html> \
							<body> \
								<h1>Hello there jsarabia</h1> \
								<p>Esto ya empieza a funcionar!!</p> \
								<img src='https://external-content.duckduckgo.com/iu/?u=http%3A%2F%2Fwww.geek-officiel.com%2Fwp-content%2Fuploads%2F2015%2F06%2Fgecko.jpg&f=1&nofb=1&ipt=023bd0e6eca896e1ca26e9ef3728562e266cae00740acbb20c90f04a3dcca9ba&ipo=images' width='530' height='400'> \
							</body> \
						</html>";
						int writeVal = write(it->fd, response.c_str(), response.length());
						if (writeVal == -1)
							raiseError("error writing data");
						close(it->fd);
						it = clients.erase(it);
						//it++;
					}
				}
			}
		}
		std::cout << clients.size() << std::endl;
	}
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
