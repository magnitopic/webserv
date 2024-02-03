/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:39 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/03 16:53:39 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

#include <iostream>
#include <fstream>
#include <cstring>
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
#include <sstream>

#include "enums.hpp"
#include "Socket.hpp"
#include "Server.hpp"
//#include "Request.hpp"

int			setAction(std::string);
void		parseReq(std::string req);
std::string	configToString(char *str);
void		raiseError(const char *msg);
std::string getFile(const std::string &fileAdr);
int			parseConfigFile(std::string file_name);
int			isAllowed(Server server, const std::string str, std::list<std::string> actions, std::list<std::string> forbidden);
void		handleRequests(Socket &socketClass, char *buffer, Server &server, std::string str);
std::string	getContentType(const std::string &extension);
void		createConection(std::string str);

#endif
