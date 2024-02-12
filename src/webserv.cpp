/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/12 12:41:17 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

// TODO: check for conflicting ports when parsing the config file

/*
	steps for creating connections:

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
		if (poll(clients.data(), clients.size(), -1) == -1)
			raiseError("error polling data");

		// iterate through the clients and remove connection if no read value
		std::vector<pollfd>::iterator it = clients.begin();
		while (it != clients.end())
		{
			if (it->revents == POLLIN)
			{
				char buffer[1024];		// This size of 1024 is temporary, we can set 1024 by default but it can also be specified in the config file
				int readVal = recv(it->fd, buffer, sizeof(buffer), 0);
				if (readVal == -1)
					raiseError("error reading data");
				else if (readVal == 0)
				{
					close(it->fd);
					it = clients.erase(it);
					continue;
				}
				Request req = parseReq(buffer);
				req.setReqBuffer(buffer);
				std::string aux(buffer, readVal);
				req.setContentLength();
				Response response;
				if (req.getContentLength() > 1023)  // This 1023 is temporary and only if it was not set in the config file with another value
				{
					response.generateResponse(413, response.getErrorMsg(413), server);
					response.setContentLength(response.getResponse());
					response.generateHeader(413, server);
				}
				else
				{
					while (static_cast<int>(aux.length()) < req.getContentLength()){
						readVal = recv(it->fd, buffer, sizeof(buffer), 0);
						aux += buffer;
					}
				}
				req.setReqBuffer(const_cast<char *>(aux.c_str()));
				Location location(aux.substr(aux.find("/"), aux.find(" HTTP") - aux.find(" ") - 1));
				location.setBuffer(str);
				location.setActions(server, str);
				location.setForbidden();
				location.setIndex();
				req.setAbsPath(server);
				if (req.getMethod() == "GET" || req.getMethod() == "POST" || req.getMethod() == "DELETE")
				{
					if (!isAllowed(server, req.getMethod(), location.getActions(), location.getForbidden()))
					{
						response.generateResponse(405, response.getErrorMsg(405), server);
						response.setContentLength(response.getResponse());
						response.generateHeader(405, server);
					}
					else{
						if (req.getMethod() == "GET")
							handleRequests(location, server, req, response);
						else if (req.getMethod() == "POST")
							handlePost(location, server, req, response);
						else if (req.getMethod() == "DELETE")
							deleteMethod(server, req, response);
					}
				}
				else
				{
					response.generateResponse(501, response.getErrorMsg(501), server);
					response.setContentLength(response.getResponse());
					response.generateHeader(501, server);
				}
				std::string resp = response.generateHttpResponse();
				int writeVal = write(it->fd, resp.c_str(), resp.length());
				if (writeVal == -1)
					raiseError("error writing data");
				close(it->fd);
				it = clients.erase(it);
				server.emptyActions();
				location.emptyActions();
			}
		}
	}
}

void handleRequests(Location &location, Server &server, Request &req, Response &resp)
{
	location.setAutoIndex(isAutoindex(location));
	/*
		The options for a response are:
		- if the petition is a file and it exists, return the file
		- if the petition is a directory and it exists, check if there is a default file (like index.html)
		- if the petition is a directory and it exists, render the auto-index page
		- if the petition does not exist, return a 404 error
	 */
	struct stat s;
	req.setAbsPath(server);
	req.setExtension();
	if (stat(req.getAbsPath().c_str(), &s) == 0 && s.st_mode & S_IFREG)
	{
		resp.setResponse(getFile(req.getAbsPath()));
		resp.setContentLength(resp.getResponse());
		resp.generateHeader(200, server);
		req.setContentType(parseContentType(req.getExtension()));
		resp.generateHeaderContent(200, req.getContentType(), server);
	}
	else if (access(req.getAbsPath().c_str(), F_OK) == 0 &&
			 stat((server.getRoot() + location.getIndex()).c_str(), &s) == 0 && S_ISREG(s.st_mode))
	{
		resp.setResponse(getFile(server.getRoot() + location.getIndex()));
		resp.setContentLength(resp.getResponse());
		req.setContentType(parseContentType("html"));
		resp.generateHeaderContent(200, req.getContentType(), server);
	}
	else if (location.getAutoIndex() == true)
	{
		location.generateAutoIndex(server, location.getDirectory(), location, resp);
		resp.setContentLength(resp.getResponse());
		resp.generateHeader(200, server);
	}
	else
	{
		resp.setResponseNotFound();
		resp.setContentLength(resp.getResponse());
		req.setContentType(parseContentType("html"));
		resp.generateHeader(404, server);
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
