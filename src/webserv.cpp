/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/01 14:55:19 by jsarabia         ###   ########.fr       */
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

static void closeConnections(struct pollfd fds[200], int nfds)
{
	for (int i = 0; i < nfds; i++)
	{
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}
}

static void justWaiting(std::vector<Server> &servers, std::vector<Socket> sockets, struct pollfd fds[200], std::string configFile)
{
	bool end_server = false;
	bool close_conn = false;
	bool compress_array = false;
	char buffer[1024];
	std::string finalBuf;
	int rc = 0;
	int new_sd = 0;
	int nfds = sockets.size();

	memset(buffer, 0, sizeof(buffer));
	while (end_server == false)
	{
		rc = poll(fds, nfds, TIMEOUT);
		if (rc < 0)
		{
			perror("poll() failed");
			break;
		}
		else if (rc == 0)
		{
			cerr << "Timeout. End program" << endl;
			break;
		}
		int current_size = nfds;
		for (int i = 0; i < current_size; i++)
		{
			if (fds[i].revents == 0)
				continue;
			if (i < static_cast<int>(sockets.size()) && fds[i].fd == sockets[i].getListen_sd())
			{

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

					fds[nfds].fd = new_sd;
					fds[nfds].events = POLLIN;
					nfds++;
				}
			}
			else
			{
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
				if ((static_cast<int>(bodyReq(finalBuf).length()) >= parsedContentLength(finalBuf) && parsedContentLength(finalBuf) > 0) || (strncmp(finalBuf.substr(0, 4).c_str(), "POST", 4) && finalBuf.find("\r\n\r\n") < finalBuf.length() && finalBuf.find("\r\n\r\n") > 0))
				{
					client cl;
					cl.fd = fds[i].fd;
					cl.finalbuffer = finalBuf;
					handleRequests(servers, cl, configFile);
					close_conn = true;
				}
				memset(buffer, 0, sizeof(buffer));
				if (close_conn)
				{
					finalBuf.clear();
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

Socket createConection(unsigned int port)
{
	Socket socket;

	socket.createSocket();
	socket.bindSocket(port);
	socket.listenSocket();
	return socket;
}

void handleRequests(std::vector<Server> &servers, client &clients, std::string str)
{
	Request req = parseReq(clients.finalbuffer);
	req.setReqBuffer(clients.finalbuffer);
	req.setHost();
	req.setPort();
	int i = fixingCPP(servers, req);
	if (i >= static_cast<int>(servers.size()))
	{
		i = 0;
		i = fixingCPPAgain(servers, req);
	}
	if (req.getPort() == 0)
		req.newSetPort(servers[0].getPorts()[0]);
	if (i >= static_cast<int>(servers.size()))
		i = 0;
	servers[i].setActions(str);
	std::string aux = clients.finalbuffer;
	req.setContentLength();
	servers[i].setMaxClientSize(str);
	Response response;
	if (req.getContentLength() > static_cast<int>(servers[i].getMaxClientSize()))
	{
		response.setErrorCode(413);
		response.generateResponse(413, response.getErrorMsg(413), servers[i]);
		response.setContentLength(response.getResponse());
		response.generateHeader(413, servers[i]);
	}
	std::string temp;
	if (req.getMethod() == "DELETE")
	{
		temp = aux.substr(aux.find("/"), aux.find(" HTTP") - aux.find(" ") - 1);
		if (temp.rfind("/") == 0)
			temp = temp.substr(0, temp.rfind("/") + 1);
		else
			temp = temp.substr(0, temp.rfind("/"));
	}
	else
		temp = aux.substr(aux.find("/"), aux.find(" HTTP") - aux.find(" ") - 1);
	Location location(temp);
	location.setValues(str);
	if (location.setRedirection())
	{
		response.setErrorCode(location.getRedirection().begin()->first);
		response.generateRedirection(location); // Crete new function to redirect to the selected URL
		response.setContentLength(response.getResponse());
		response.generateRedirectHeader(location, servers[i]);
	}
	req.setAbsPath(servers[i]);
	if ((req.getMethod() == "GET" || req.getMethod() == "POST" || req.getMethod() == "DELETE") && response.getErrorCode() < 90)
	{
		if (!isAllowed(servers[i], req, location))
		{
			response.setErrorCode(405);
			response.generateResponse(405, response.getErrorMsg(405), servers[i]);
			response.setContentLength(response.getResponse());
			response.generateHeader(405, servers[i]);
		}
		else
		{
			if (req.getMethod() == "GET")
				getMethod(location, servers[i], req, response);
			else if (req.getMethod() == "POST")
				handlePost(servers[i], req, response);
			else if (req.getMethod() == "DELETE")
				deleteMethod(servers[i], req, response);
		}
	}
	else if (response.getErrorCode() < 100)
	{
		response.setErrorCode(501);
		response.generateResponse(501, response.getErrorMsg(501), servers[i]);
		response.setContentLength(response.getResponse());
		response.generateHeader(501, servers[i]);
	}
	std::string resp = response.generateHttpResponse();
	cout << resp << endl;
	int writeVal = send(clients.fd, resp.c_str(), resp.length(), 0);
	if (writeVal < 1)
		raiseError("send() failed");
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
	std::vector<int> openPorts;
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::vector<unsigned int> ports = it->getPorts();
		for (std::vector<unsigned int>::iterator it2 = ports.begin(); it2 != ports.end(); it2++)
		{
			if (find(openPorts.begin(), openPorts.end(), *it2) == openPorts.end())
			{
				Socket aux = createConection(*it2);
				sockets.push_back(aux);
				fds[i].fd = aux.getListen_sd();
				fds[i].events = POLLIN;
				i++;
				openPorts.push_back(*it2);
			}
		}
	}
	justWaiting(servers, sockets, fds, file);
	return 0;
}
