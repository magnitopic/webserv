/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/21 14:44:19 by jsarabia         ###   ########.fr       */
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

/*static int	readRequest(std::vector<client> clients, std::string str, std::vector<Server> servers, std::vector<int> connections, int fd)
{
	// iterate through the clients and handle requests
	for (int i = 0; i < static_cast<int>(clients.size()); i++)
	{
		memset(clients[i].buf, 0, 10000);
		if (read(clients[i].fd, clients[i].buf, sizeof(clients[i].buf)) == -1){
			close(clients[i].fd);
			clients.erase(clients.begin() + i);
			return -1;
		}
			//raiseError("error reading data");
		clients[i].finalbuffer += clients[i].buf;
		cout << clients[i].finalbuffer << endl;
		if (clients[i].finalbuffer.find("\r\n\r\n") && clients[i].finalbuffer.find("\r\n\r\n") <= clients[i].finalbuffer.length())
		{
			std::string aux = clients[i].finalbuffer.substr(clients[i].finalbuffer.find("\r\n\r\n"), clients[i].finalbuffer.length() - clients[i].finalbuffer.find("\r\n\r\n"));
			if (static_cast<int>(aux.length()) >= parsedContentLength(clients[i].finalbuffer) || parsedContentLength(clients[i].finalbuffer) < 0){
				cout << "aux len: " << aux.length() << endl;
				cout << "parsed len: " << parsedContentLength(clients[i].finalbuffer) << endl;
				cout << clients[i].finalbuffer << endl;
				handleRequests(i, servers[0], clients, str);
				close(clients[i].fd);
				clients.erase(clients.begin() + i);
				i--;
				connections.erase(std::find(connections.begin(), connections.end(), fd));
				//continue;
			}
			else if (greatExpectations(clients[i].finalbuffer))
			{
				cout << "we are having great expectations" << endl;
				send(clients[i].fd, "HTTP/1.1 100 Continue\n\n", 23, 0);
			}
		}
	}
	return 0;
}*/

void createConection(std::string str)
{
	std::vector<Server> servers;
	Socket				socket;

	/**
	 * Parse the config file and store the servers in a vector
	 * Server should contain a list of all the ports it listens to
	 * and a list of all the directories it has configured
	 */
	//servers = parseConfigFile(str);

	(void)str;
	socket.createSocket();
	socket.bindSocket(servers);
	socket.listenSocket();
	socket.initializePollfdStruct();
	socket.justWaiting();
}

void handleRequests(int clientPos, Server &server, std::vector<client> clients, std::string str)
{
	Request req = parseReq(clients[clientPos].finalbuffer);
	req.setReqBuffer(clients[clientPos].finalbuffer);
	std::string aux = clients[clientPos].finalbuffer;
	req.setContentLength();
	server.setMaxClientSize(str);
	Response response;
	if (req.getContentLength() > static_cast<int>(server.getMaxClientSize()))
	{
		response.setErrorCode(413);
		response.generateResponse(413, response.getErrorMsg(413), server);
		response.setContentLength(response.getResponse());
		response.generateHeader(413, server);
	}
	Location location(aux.substr(aux.find("/"), aux.find(" HTTP") - aux.find(" ") - 1));
	location.setValues(str);
	if (location.setRedirection())
	{
		response.setErrorCode(location.getRedirection().begin()->first);
		response.generateRedirection(location); // Crete new function to redirect to the selected URL
		response.setContentLength(response.getResponse());
		response.generateRedirectHeader(location, server);
	}
	req.setAbsPath(server);
	if ((req.getMethod() == "GET" || req.getMethod() == "POST" || req.getMethod() == "DELETE") && response.getErrorCode() < 90)
	{
		if (!isAllowed(server, req, location))
		{
			response.setErrorCode(405);
			response.generateResponse(405, response.getErrorMsg(405), server);
			response.setContentLength(response.getResponse());
			response.generateHeader(405, server);
		}
		else
		{
			if (req.getMethod() == "GET")
				getMethod(location, server, req, response);
			else if (req.getMethod() == "POST")
				handlePost(location, server, req, response);
			else if (req.getMethod() == "DELETE")
				deleteMethod(server, req, response);
		}
	}
	else if (response.getErrorCode() < 100)
	{
		response.setErrorCode(501);
		response.generateResponse(501, response.getErrorMsg(501), server);
		response.setContentLength(response.getResponse());
		response.generateHeader(501, server);
	}
	std::string resp = response.generateHttpResponse();
	int writeVal = write(clients[clientPos].fd, resp.c_str(), resp.length());
	if (writeVal == -1)
		raiseError("error writing data");
	close(clients[clientPos].fd);
	clients.erase(clients.begin() + clientPos);
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
