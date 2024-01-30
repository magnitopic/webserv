/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:39 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/30 20:00:06 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
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

void		raiseError(const char *msg);
int			parseConfigFile(std::string file_name);
std::string	configToString(char *str);
std::string	getFile(std::string fileAdr);
int			setAction(std::string);
int			isAllowed(const std::string str, std::list<std::string> actions);
void handleRequests(Socket &socketClass, char *buffer, Server &server);

#endif
