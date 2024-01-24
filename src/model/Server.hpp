/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 12:42:38 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/24 14:46:39 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "../../include/webserv.hpp"
#include "Location.hpp"

class Server
{
private:
	unsigned int port; // listen
	std::string name; // server_name
	std::string root; // root
	std::string index; // index
	std::string host; // host
	std::map<int, std::string> errorPages; // error_page
	unsigned long maxClientBodySize; // client_max_body_size
	std::vector<Location> locations; // location
	

public:
	Server();
	Server(const Server &copy);
	Server &operator=(const Server &assign);
	~Server();
};

#endif
