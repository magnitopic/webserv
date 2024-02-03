/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 12:10:40 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/03 16:09:42 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP_
#define SERVER_HPP_

class Server;

#include "webserv.hpp"
//#include "Location.hpp"

class Server
{
private:
	unsigned int				port; // listen
	std::string					name; // server_name
	std::string					root; // root
	std::string					index; // index
	std::string					host; // host
	std::list<std::string>		actions; //actions all
	std::map<int, std::string>	errorPages; // error_page
	unsigned long				maxClientBodySize; // client_max_body_size
	//std::vector<Location>		locations; // location


public:
	Server();
	Server(const Server &copy);
	Server &operator=(const Server &assign);
	~Server();
	void						setPort(std::string str);
	void						setName(std::string str);
	void						setActions(std::string str);
	void						setRoot(std::string str);
	int							getPort(void);
	std::string					getRoot(void);
	std::string					getName(void);
	std::list<std::string> 		getActions(void);
	void						emptyActions(void);
};

#endif
