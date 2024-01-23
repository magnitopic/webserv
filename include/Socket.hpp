/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:58:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/01/23 17:21:44 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <map>
#include <list>
#include <string>

class Socket
{
	private:
		std::string				directory;
		std::list<std::string>	actions;
		std::list<std::string>	forbidden;
		std::string				response;
	public:
		Socket(void);
		Socket(std::string directory);
		Socket(const Socket &socket);
		Socket &operator=(const Socket &socket);
		~Socket(void);
		void		setDirectory(std::string directory);
		std::string	getDirectory(void);
		void		setActions(std::string directory, std::string text);
		void		setForbidden(std::string directory, std::string text);
};

std::ostream &operator<<(std::ostream &os, std::list<std::string> &list);

#endif
