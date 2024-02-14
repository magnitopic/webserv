/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:39 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/13 16:22:40 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

#include <netdb.h>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/stat.h>
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

#include "enums.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include "PostReq.hpp"
using namespace std;

#define MAX_SERVERS 1024

int					setAction(std::string);
Request				parseReq(std::string passedReq);
std::string			configToString(char *str);
void				raiseError(const char *msg);
std::string			getFile(const std::string &fileAdr);
std::vector<Server>	parseConfigFile(std::string file_name);
int					isAllowed(Server &server, Request & req, Location &location);
void				handleRequests(int clientFd, Server &server, std::string buffer, std::vector<int> clients, std::string str);
std::string			parseContentType(std::string extension);
void				createConection(std::string str);
void				getMethod(Location &location, Server &server, Request &req, Response &resp);
void				handlePost(Location &location, Server &server, Request &req, Response &response);
std::string			deleteFirstElement(std::string str);
void				deleteMethod(Server &server, Request &req, Response &resp);
void				showData(Request &req, Response &response);

#endif
