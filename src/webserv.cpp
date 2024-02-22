/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/22 15:02:09 by alaparic         ###   ########.fr       */
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

static void	closeConnections(struct pollfd fds[200], int nfds)
{
	for (int i = 0; i < nfds; i++){
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}
}

static void	justWaiting(Socket &socket, struct pollfd fds[200])
{
	bool		end_server = false;
	bool		close_conn = false;
	bool		compress_array = false;
	char		buffer[80];
	std::string	finalBuf;

	memset(buffer, 0, sizeof(buffer));
	while (end_server == false)
	{
		cout << "Waiting on poll()..." << endl;
		socket.setRc(poll(fds, socket.getNfds(), socket.getTimeout()));
		if (socket.getRc() < 0){
			perror("poll() failed");
			break;
		}
		else if (socket.getRc() == 0){
			cout << "Timeout. End program" << endl;
			break;
		}
		int current_size = socket.getNfds();
		for (int i = 0; i < current_size; i++){
			if (fds[i].revents == 0)
				continue;
			if (fds[i].revents != POLLIN){
				cerr << "Error! revents = " << fds[i].revents << endl;
				end_server = true;
				break;
			}

			if (fds[i].fd == socket.getListen_sd())
			{
				cout << "Listening socket is readable" << endl;

				// Accept all queued incoming connections

				socket.setNew_sd(0);
				while (socket.getNew_sd() != -1){
					socket.setNew_sd(accept(socket.getListen_sd(), NULL, NULL));
					if (socket.getNew_sd() < 0){
						if (errno != EWOULDBLOCK){
							perror("accept() failed");
							end_server = true;
						}
						break;
					}

				// Adding incoming connection to the pollfd structure

					cout << "New incoming connection" << endl;
					fds[socket.getNfds()].fd = socket.getNew_sd();
					fds[socket.getNfds()].events = POLLIN;
					socket.increaseNfds();
				}
			}
			else{
				cout << "Descriptor " << fds[i].fd << " is readable" << endl;
				close_conn = false;
				socket.setRc(recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0));
				if (socket.getRc() < 0){
					if (errno != EWOULDBLOCK){
						perror("recv() failed");
						close_conn = true;
					}
				}
				finalBuf += buffer;
				if (socket.getRc() == 0 || (static_cast<int>(bodyReq(finalBuf).length()) >= parsedContentLength(finalBuf) && parsedContentLength(finalBuf) > 0)
					|| (finalBuf.substr(0, 4) != "POST" && finalBuf.find("\r\n\r\n") < finalBuf.length() && finalBuf.find("\r\n\r\n") > 0)){
					cout << "|" << finalBuf << "|" << endl;
					cout << "Connection closed" << endl;
					close_conn = true;
				}
				memset(buffer, 0, sizeof(buffer));
				if (close_conn)
				{
					socket.setRc(send(fds[i].fd, finalBuf.c_str(), finalBuf.size(), 0));
					if (socket.getRc() == 0){
						perror("send() failed");
						close_conn = true;
					}
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_array = true;
				}
			}
		}
		if (compress_array){
			compress_array = false;
			for (int i = 0; i < socket.getNfds(); i++){
				if (fds[i].fd == -1){
					for (int j = i; j < socket.getNfds(); j++)
						fds[j].fd = fds[j + 1].fd;
					i--;
					socket.decrementNfds();
				}
			}
		}
	}
	closeConnections(fds, socket.getNfds());
}


void createConection(std::string str, int i) // The value of i is the counter in which we will be iterating this function
{
	std::vector<Server> servers;
	Socket				socket;
	struct pollfd		fds[200];

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
	memset(fds, 0, sizeof(fds));
	fds[i].fd = socket.getListen_sd();
	fds[i].events = POLLIN;
	socket.setTimeout(3 * 60 * 1000);
	justWaiting(socket, fds);
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
		createConection(file, 0);		// TODO: 0 is temporaray, shoudl be what server needs the connection
		sleep(1);
	}
	return 0;
}
