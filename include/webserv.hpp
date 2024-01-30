/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:39 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/30 12:17:50 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include "enums.hpp"
#include "Socket.hpp"
#include "Server.hpp"

void			raiseError(const char *msg);
int				parseConfigFile(std::string file_name);
std::string		configToString(char *str);
std::string		getFile(std::string fileAdr);
int				setAction(std::string);
int				isAllowed(const std::string str, std::list<std::string> actions);

#endif
