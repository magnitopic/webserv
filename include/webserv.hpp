/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:39 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/15 16:35:45 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

#include <netdb.h>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <poll.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "enums.hpp"
#include "Socket.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "PostReq.hpp"

using namespace std;

#define MAX_SERVERS 1024
#define MAX_CGI_TIME 5
// colours

#define RED		"\033[0;31m"
#define GREEN	"\033[0;32m"
#define YELLOW	"\033[0;33m"
#define BLUE	"\033[0;34m"
#define PURPLE	"\033[0;35m"
#define CYAN	"\033[0;36m"
#define RESET	"\033[0m"

struct	client
{
	unsigned int fd;
	std::string finalbuffer;
};

int											setAction(std::string);
Request										parseReq(std::string passedReq);
std::string									configToString(char *str);
void										raiseError(const char *msg);
std::string									getFile(const std::string &fileAdr);
void										parseConfigFile(std::string file_name, std::vector<Server> &serverList);
int											isAllowed(Server &server, Request & req, Location &location);
void 										handleRequests(std::vector<Server> &servers, client& clients, std::string str);
std::string									parseContentType(std::string extension);
void										createConection(std::string str, int i);
void										getMethod(Location &location, Server &server, Request &req, Response &resp);
void										handlePost(Server &server, Request &req, Response &response);
std::string									deleteFirstElement(std::string str);
void										deleteMethod(Server &server, Request &req, Response &resp);
void										showData(Request &req, Response &response);
std::vector<std::string>					splitServerDirectives(std::string configFile);
std::vector<std::string>					splitLocationDirectives(std::string serverDirectives);
int											parsedContentLength(std::string str);
int											greatExpectations(std::string str);
void										continueConnecting();
std::string									bodyReq(std::string str);
struct pollfd								*initializePollfdStruct(struct pollfd fds[200], int listen_sd);
std::map<int, std::string>::const_iterator	getMapIterator(const std::map<int, std::string> &map, int code);
int											fixingCPP(std::vector<Server> &servers, Request& req);
int											fixingCPPAgain(std::vector<Server> &servers, Request& req);
int											myOwnFindVector(std::vector<unsigned int> vector, unsigned int num);
bool										cgiForPostReq(PostReq& post, Request &req, Response &resp, Server &server);
bool										cgiForGetReq(Request &req, Response &resp, Server &server);
int											fixingMapOperator(std::map<int, std::string> map, int code);
std::string									vectorToString(std::vector<char> vector);

#endif