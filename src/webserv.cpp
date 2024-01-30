/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/30 18:57:13 by jsarabia         ###   ########.fr       */
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
	Socket			socketClass;
	Server			server;
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
	server.setPort(str);
	server.setName(str);
	serverScruct.sin_port = htons(server.getPort());

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
						int action = setAction(buffer);
						std::string aux = buffer;
						socketClass.setDirectory(aux.substr(aux.find("/"), aux.find(" HTTP") - aux.find(" ") - 1)); // Now we should check if the action can be performed in the chosen directory, if not thwrow error ¿405?
						socketClass.setActions(socketClass.getDirectory(), str);
						socketClass.setForbidden(socketClass.getDirectory(), str);
						std::string act;
						if (action < 3)
							act = socketClass.getActionsArray(action);
						else
							act = "";
						if (act.length() > 0)
						{
							if (!isAllowed(act, socketClass.getActions())){
								socketClass.setResponse("<html>\n<head><title>405 Not Allowed</title></head>\n<body>\n<center><h1>405 Not Allowed</h1></center>\n<hr><center>" + server.getName() + "</center>\n</body>\n</html>");
								socketClass.setContentLength(socketClass.getResponse());
								socketClass.setHeader("HTTP/1.1 405 Method Not Allowed\nServer: " + server.getName() + "\nContent-Type: text/html; charset=utf-8\nContent-Length: " + socketClass.getContentLength().c_str() +"\n\n");
							}
							else{
								if (socketClass.getDirectory().compare("/") == 0){
									socketClass.setResponse(getFile("pages/index.html"));
									socketClass.setContentLength(socketClass.getResponse());
									socketClass.setHeader("HTTP/1.1 200 OK\nServer: " + server.getName() + "\nContent-Type: text/html; charset=utf-8\nContent-Length: " + socketClass.getContentLength().c_str() +"\n\n");
								}
								/*else if (socketClass.getDirectory().compare("/favicon.ico") == 0)
									socketClass.setResponse("HTTP/1.1 200 OK\nServer: " + server.getName() + "\nContent-Type: text/html; charset=utf-8\n\n" +
												getFile("images/favicon.ico"));*/
								else if (std::string(buffer).find("GET /info HTTP/1.1") != std::string::npos){
									socketClass.setResponse(getFile("pages/info/geco.html"));
									socketClass.setContentLength(socketClass.getResponse());
									socketClass.setHeader("HTTP/1.1 200 OK\nServer: " + server.getName() + "\nContent-Type: text/html; charset=utf-8\nContent-Length: " + socketClass.getContentLength().c_str() +"\n\n");
								}
								else if (std::string(buffer).find("GET /teapot HTTP/1.1") != std::string::npos){
									socketClass.setResponse(getFile("pages/teapot.html"));
									socketClass.setContentLength(socketClass.getResponse());
									socketClass.setHeader("HTTP/1.1 418 I'm a teapot\nServer: " + server.getName() + "\nContent-Type: text/html; charset=utf-8\nContent-Length: " + socketClass.getContentLength().c_str() +"\n\n");
								}
								else
								{
									socketClass.setResponse(getFile("pages/error_404.html"));
									socketClass.setContentLength(socketClass.getResponse());
									socketClass.setHeader("HTTP/1.1 404 Not Found\nServer: " + server.getName() + "\nContent-Type: text/html; charset=utf-8\nContent-Length: " + socketClass.getContentLength().c_str() + "\n\n");
								}
							}
						}
						else
						{
							socketClass.setResponse("<html>\n<head><title>501 Not Implemented</title></head>\n<body>\n<center><h1>501 Not Implemented</h1></center>\n<hr><center>" + server.getName() + "</center>\n</body>\n</html>");
							socketClass.setContentLength(socketClass.getResponse());
							socketClass.setHeader("HTTP/1.1 501 Not Implemented\nServer: " + server.getName() + "\nContent-Type: text/html; charset=utf-8\n\n");
						}
						int writeVal = write(it->fd, socketClass.getHeader().c_str(), socketClass.getHeader().length());
						if (writeVal == -1)
							raiseError("error writing data");
						writeVal = write(it->fd, socketClass.getResponse().c_str(), socketClass.getResponse().length());
						//std::cout << socketClass.getResponse() << std::endl;
						if (writeVal == -1)
							raiseError("error writing data");
						close(it->fd);
						it = clients.erase(it);
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
