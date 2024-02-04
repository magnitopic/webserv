/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:46:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/04 19:29:41 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

void raiseError(const char *msg)
{
	perror(msg);
	exit(1);
}

std::string	configToString(char *str)
{
	std::ifstream	configFile;
	std::string		file;

	configFile.open(str, std::ifstream::in);
	if (!configFile.is_open())
		raiseError("error opening the input file");
	while (configFile.good()){
		char c = configFile.get();
		file.push_back(c);
	}
	configFile.close();
	return file;
}

int	isAllowed(Server &server, std::string str, std::list<std::string> actions, std::list<std::string> forbidden)
{
	if (forbidden.size() > 1){
		std::list<std::string>::iterator it = std::find(forbidden.begin(), (std::prev(forbidden.end())), str);
		if (it != forbidden.end())
			return 0;
	}
	else if (forbidden.size() == 1){
		if (str == *forbidden.begin())
			return 0;
	}
	if (actions.size() > 1){
		std::list<std::string>::iterator it = std::find(actions.begin(), (std::prev(actions.end())), str);
		if (it != actions.end())
			return 1;
	}
	else if (actions.size() == 1){
		if (str == *actions.begin())
			return 1;
	}
	if (server.getActions().size() > 1){
		std::list<std::string>::iterator it = std::find(server.getActions().begin(), (std::prev(server.getActions().end())), str);
		if (it != server.getActions().end())
			return 1;
	}
	else if (server.getActions().size() == 1){
		if (str == *server.getActions().begin())
			return 1;
	}
	return 0;
}
