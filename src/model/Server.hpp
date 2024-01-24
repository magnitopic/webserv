/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 12:42:38 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/24 12:54:18 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "../../include/webserv.hpp"

class Server
{
private:
	unsigned int port;
	std::string name;
	std::map<int, std::string> errorPages;
	unsigned long maxClientBodySize;
	

public:
	Server();
	Server(const Server &copy);
	Server &operator=(const Server &assign);
	~Server();
};

#endif
