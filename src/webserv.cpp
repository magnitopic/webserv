/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/07 20:18:43 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

/*
	steps for creating contections:

	-	Create socket
	-	Bind socket
	-	Listen socket
	-	Accept and receive data
	-	Disconnect
 */

void createConection(std::string str)
{
	// Create socket
	Server server;
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
	server.setRoot(str);
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
		server.setActions(str);
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
			// iterate through the clients and remove connection if no read value
			std::vector<pollfd>::iterator it = clients.begin();
			while (it != clients.end())
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
						it = clients.erase(it);
						continue;
					}
					Socket socketClass;
					Request req = parseReq(buffer);
					Response	response;
					std::string aux = buffer;
					socketClass.setDirectory(aux.substr(aux.find("/"), aux.find(" HTTP") - aux.find(" ") - 1));
					socketClass.setActions(server, socketClass.getDirectory(), str);
					socketClass.setForbidden(socketClass.getDirectory(), str);
					if (req.getMethod() == "GET" || req.getMethod() == "POST" || req.getMethod() == "DELETE")
					{
						if (!isAllowed(server, req.getMethod(), socketClass.getActions(), socketClass.getForbidden()))
						{
							response.generateResponse(405, response.getErrorMsg(405), server);
							response.setContentLength(response.getResponse());
							response.generateHeader(405, response.getErrorMsg(405), server);
						}
						else
							handleRequests(socketClass, buffer, server, str, req, response);
					}
					else
					{
						response.generateResponse(501, response.getErrorMsg(501), server);
						response.setContentLength(response.getResponse());
						response.generateHeader(501, response.getErrorMsg(501), server);
					}
					std::string resp = response.generateHttpResponse(req.uri);
					int writeVal = write(it->fd, resp.c_str(), resp.length());
					if (writeVal == -1)
						raiseError("error writing data");
					close(it->fd);
					it = clients.erase(it);
					server.emptyActions();
					socketClass.emptyActions();
				}
			}
		}
	}
}

void handleRequests(Socket &socketClass, char *buffer, Server &server, std::string str, Request &req, Response &response)
{
	(void)buffer;
	socketClass.setAutoIndex(isAutoindex(str, socketClass));
	if (socketClass.getAutoIndex() == true)
	{
		socketClass.generateAutoIndex(server, socketClass.getDirectory(), socketClass, response);
		response.setContentLength(response.getResponse());
		response.generateHeader(200, response.getErrorMsg(200), server);
	}
	else // ! this code should be changed but it will serve as backup for now
	{
		struct stat s;
		string aux = server.getRoot() + req.uri; // TODO: This could go inside the request class
		if (stat(aux.c_str(), &s) == 0 && s.st_mode & S_IFREG)
		{
			response.setResponse(getFile(aux));
			response.setContentLength(response.getResponse());
			response.generateHeader(200, response.getErrorMsg(200), server);
			req.setContentType(parseContentType(req.extension));
			response.generateHeaderContent(200, req.getContentType(), server);
		}
		else if (!access(aux.c_str(), F_OK))
		{
			response.setResponse(getFile(aux));
			response.setContentLength(response.getResponse());
			req.setContentType(parseContentType(aux));
			response.generateHeaderContent(200, req.getContentType(), server);
		}
		else
		{
			response.setResponseNotFound();
			response.setContentLength(response.getResponse());
			req.setContentType(parseContentType("html"));
			response.generateHeader(404, response.getErrorMsg(404), server);
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
	while (true)
	{
		createConection(file);
		sleep(1);
	}
	return 0;
}
