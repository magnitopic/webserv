/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:46:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/24 18:48:21 by jsarabia         ###   ########.fr       */
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

int	setPort(std::string str)
{
	std::size_t	found = str.find("listen") + 6;
	std::string	num;
	if (found > str.length())
		raiseError("no port appear in configuration file");
	while (isspace(str[found]))
		found++;
	while (found < str.length()){
		if (isdigit(str[found]))
			num.push_back(str[found]);
		else if (isspace(str[found]))
			return std::atoi(num.c_str());
		else
			raiseError("error in configuration file");
		found++;
	}
	return std::atoi(num.c_str());
}


int	isAllowed(const std::string str, std::list<std::string> actions)
{
	if (actions.size() > 1){
		std::list<std::string>::iterator it = std::find(actions.begin(), (std::prev(actions.end())), str);
		if (it != actions.end())
			return 1;
	}
	else if (actions.size() == 1){
		if (str == *actions.begin())
			return 1;
	}
	return 0;
}
