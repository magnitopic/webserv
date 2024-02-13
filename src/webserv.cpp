/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/13 12:58:13 by jsarabia         ###   ########.fr       */
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
	std::vector<Server> servers;

	/**
	 * Parse the config file and store the servers in a vector
	 * Server should contain a list of all the ports it listens to
	 * and a list of all the directories it has configured
	 */
	// servers = parseConfigFile(str);

	// ! These two lines are temporary
	servers.push_back(Server(str));
	servers[0].setActions(str);

	std::cout << "\033[0;34m==> \033[0;36mWebserv running ✅\n\033[0;34m==>\033[0;36m And listening on these ports:\033[0;33m" << std::endl;
	// Create a socket for every port. Each server can have multiple ports
	std::vector<pollfd> fds;
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::vector<unsigned int> ports = it->getPorts();
		for (std::vector<unsigned int>::iterator it2 = ports.begin(); it2 != ports.end(); it2++)
		{
			Socket newSocket(*it2);
			struct pollfd pfd;
			pfd.fd = newSocket.getSocketFD();
			pfd.events = POLLIN;
			fds.push_back(pfd);
			// add socket to this servers socket list
			it->addSocket(newSocket);
			std::cout << "\t" << *it2 << std::endl;
		}
	}
	std::cout << "\033[0m----------------------------------" << std::endl;

	// Main loop that handles connections
	while (true)
	{
		// polling data from clients
		int pollVal = poll(&fds[0], fds.size(), -1);
		if (pollVal == -1)
			raiseError("error polling data");

		// iterate through the servers and accept new connections
		std::vector<int> clients;
		for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
		{
			if (it->revents == POLLIN)
			{
				int socket = accept(it->fd, NULL, NULL);
				if (socket == -1)
					raiseError("error accepting data");
				// add new connection to the clients list
				clients.push_back(socket);
			}
		}

		// iterate through the clients and handle requests
		for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			char buffer[8000]; // This size of 8000 is temporary, we can set 8000 by default but it can also be specified in the config file
			int readVal = recv(*it, buffer, sizeof(buffer), 0);
			if (readVal == -1)
				raiseError("error reading data");
			else if (readVal == 0)	// ! disconnecting clients like this is temporary, we should check keep-alive
			{
				close(*it);
				it = clients.erase(it);
				continue;
			}
			handleRequests(*it, servers[0], buffer, clients, readVal, str); // ! temporary, server should be the server that handles the request
		}
	}
}

void handleRequests(int clientFd, Server &server, char *buffer, std::vector<int> clients, int readVal, std::string str)
{
	Request req = parseReq(buffer);
	req.setReqBuffer(buffer);
	std::string aux(buffer, readVal);
	req.setContentLength();
	Response response;
	if (req.getContentLength() > static_cast<int>(server.getMaxClientSize()))
	{
		response.setErrorCode(413);
		response.generateResponse(413, response.getErrorMsg(413), server);
		response.setContentLength(response.getResponse());
		response.generateHeader(413, server);
	}
	else
	{
		while (static_cast<int>(aux.length()) < req.getContentLength()){
			readVal = recv(clientFd, buffer, sizeof(buffer), 0);
			aux += buffer;
		}
	}
	req.setReqBuffer(const_cast<char *>(aux.c_str()));
	Location location(aux.substr(aux.find("/"), aux.find(" HTTP") - aux.find(" ") - 1));
	location.setValues(str);
	req.setAbsPath(server);
	if ((req.getMethod() == "GET" || req.getMethod() == "POST" || req.getMethod() == "DELETE") && response.getErrorCode() != 413)
	{
		if (!isAllowed(server, req, location))
		{
			response.setErrorCode(405);
			response.generateResponse(405, response.getErrorMsg(405), server);
			response.setContentLength(response.getResponse());
			response.generateHeader(405, server);
		}
		else{
			if (req.getMethod() == "GET")
				getMethod(location, server, req, response);
			else if (req.getMethod() == "POST")
				handlePost(location, server, req, response);
			else if (req.getMethod() == "DELETE")
				deleteMethod(server, req, response);
		}
	}
	else if (response.getErrorCode() != 413)
	{
		response.setErrorCode(501);
		response.generateResponse(501, response.getErrorMsg(501), server);
		response.setContentLength(response.getResponse());
		response.generateHeader(501, server);
	}
	std::string resp = response.generateHttpResponse();
	int writeVal = write(clientFd, resp.c_str(), resp.length());
	if (writeVal == -1)
		raiseError("error writing data");
	close(clientFd);
	clients.erase(std::remove(clients.begin(), clients.end(), clientFd), clients.end());
	//server.emptyActions();
	location.emptyActions();
	showData(req, response);
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
