/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/22 17:25:01 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

static void closeConnections(struct pollfd fds[200], int nfds)
{
	for (int i = 0; i < nfds; i++)
	{
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}
}

static void justWaiting(std::vector<Socket> sockets, struct pollfd fds[200])
{
	bool end_server = false;
	bool close_conn = false;
	bool compress_array = false;
	char buffer[80];
	std::string finalBuf;
	int rc = 0;
	int new_sd = 0;
	int nfds = 1;

	memset(buffer, 0, sizeof(buffer));
	while (end_server == false)
	{
		cout << "Waiting on poll()..." << endl;
		cout << sockets[0].getListen_sd() << endl;
		rc = poll(fds, nfds, TIMEOUT);
		if (rc < 0)
		{
			perror("poll() failed");
			break;
		}
		else if (rc == 0)
		{
			cout << "Timeout. End program" << endl;
			break;
		}
		int current_size = nfds;
		for (int i = 0; i < current_size; i++)
		{
			if (fds[i].revents == 0)
				continue;
			if (fds[i].revents != POLLIN)
			{
				cerr << "Error! revents = " << fds[i].revents << endl;
				end_server = true;
				break;
			}

			if (fds[i].fd == sockets[i].getListen_sd())
			{
				cout << "Listening socket is readable" << endl;

				// Accept all queued incoming connections

				new_sd = 0;
				while (new_sd != -1)
				{
					new_sd = accept(sockets[i].getListen_sd(), NULL, NULL);
					if (new_sd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("accept() failed");
							end_server = true;
						}
						break;
					}

					// Adding incoming connection to the pollfd structure

					cout << "New incoming connection" << endl;
					fds[nfds].fd = new_sd;
					fds[nfds].events = POLLIN;
					nfds++;
				}
			}
			else
			{
				cout << "Descriptor " << fds[i].fd << " is readable" << endl;
				close_conn = false;
				rc = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (rc < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						perror("recv() failed");
						close_conn = true;
					}
				}
				finalBuf += buffer;
				if (rc == 0 || (static_cast<int>(bodyReq(finalBuf).length()) >= parsedContentLength(finalBuf) && parsedContentLength(finalBuf) > 0) || (strncmp(finalBuf.substr(0, 4).c_str(), "POST", 4) && finalBuf.find("\r\n\r\n") < finalBuf.length() && finalBuf.find("\r\n\r\n") > 0))
				{
					cout << "|" << finalBuf << "|" << endl;
					cout << "Connection closed" << endl;
					close_conn = true;
				}
				memset(buffer, 0, sizeof(buffer));
				if (close_conn)
				{
					rc = send(fds[i].fd, finalBuf.c_str(), finalBuf.size(), 0);
					finalBuf.clear();
					if (rc == 0)
					{
						perror("send() failed");
						close_conn = true;
					}
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_array = true;
				}
			}
		}
		if (compress_array)
		{
			compress_array = false;
			for (int i = 0; i < nfds; i++)
			{
				if (fds[i].fd == -1)
				{
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

Socket createConection(unsigned int port) // The value of i is the counter in which we will be iterating this function
{
	Socket socket;

	socket.createSocket();
	socket.bindSocket(port);
	socket.listenSocket();
	cout << socket.getListen_sd() << endl;
	return socket;
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
	std::vector<Server> servers;

	if (argc > 2)
		raiseError("Too many arguments");
	else if (argc == 2)
	{
		servers = parseConfigFile(argv[1]);
		file = configToString(argv[1]);
	}
	else
	{
		char *temp = strdup("webserv.conf");
		servers = parseConfigFile(temp);
		file = configToString(temp);
		free(temp);
	}
	struct pollfd fds[200];
	memset(fds, 0, sizeof(fds));
	std::vector<Socket> sockets;
	int i = 0;
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::vector<unsigned int> ports = it->getPorts();
		for (std::vector<unsigned int>::iterator it2 = ports.begin(); it2 != ports.end(); it2++)
		{
			sockets.push_back(createConection((*it2)));
			cout << "i: " << i << endl;
			cout << sockets[i].getListen_sd() << endl;
			fds[i].fd = sockets[i].getListen_sd();
			fds[i].events = POLLIN;
			i++;
		}
	}
	justWaiting(sockets, fds);
	return 0;
}
