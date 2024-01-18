/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:39 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/18 15:57:17 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <poll.h>
#include <arpa/inet.h>
#include <unistd.h>

void raiseError(const char *msg);
int parseConfigFile(std::string file_name);

#endif
