/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 11:17:21 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/08 16:04:36 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP_
#define LOCATION_HPP_

#include "webserv.hpp"

/**
 * The aim of this class is to store the information provided by the location
 * directive of the configuration file.
 * Locations indicate the different directories in which the server will be able
 * to serve the requests.
 */
class Location
{
private:
	std::string				directory;
	std::string				buffer;			// contains all the information inside this location in the config file
	std::list<std::string>	actions; 		// actions that can be performed inside the directory
	std::list<std::string>	forbidden;		// actions denied in the directory
	std::string				header;			// header given to the request
	std::string				contentType;	// type of content that will be given to the request
	std::list<std::string>	actionsArr;		// this will be a list which contents will be ["GET", "POST", "DELETE"] and will be used to compare with the enum
	std::string				root;			// indicates the root directory for the location
	std::string				contentLength;
	std::string				index;			// path of the index, if there is one in the config file
	bool					autoIndex;

public:
	// orthodox canonical form

	Location();
	Location(std::string directory);
	Location(const Location &copy);
	Location &operator=(const Location &assign);
	~Location();

	// setters and getters

	void					setBuffer(std::string configFile);
	std::string				getBuffer(void);
	void					setDirectory(std::string directory);
	std::string				getDirectory(void);
	void					setResponse(std::string response);
	std::string				getResponse(void);
	void					setForbidden();
	void					setAutoIndex(bool autoIndex);
	bool					getAutoIndex(void);
	void					setActions(Server& server, std::string text);
	std::list<std::string>	getActions(void);
	std::string				getRoot(void);
	void					setContentType(std::string type);
	std::string				getContentType(void);
	std::list<std::string>	getForbidden(void);
	void					emptyActions(void);
	std::string				getIndex();
	void					setIndex();
	// methods

	std::string	generateHttpResponse(void);
	void		generateAutoIndex(Server &server, std::string route, Location &location, Response &response);
};

bool			isAutoindex(Location &location);
std::ostream	&operator<<(std::ostream &os, std::list<std::string> list);  //instead of creating a printList function I created this operator overload function that works the same way

#endif
