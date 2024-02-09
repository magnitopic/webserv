/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 18:18:14 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/09 18:38:25 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

void	handlePost(Location &location, Server &server, Request &req, Response &response)
{
	(void)location;
	(void)server;
	(void)response;
	int pos = req.getReqBuffer().find("Content-Type:") + 14;
	if (pos >= static_cast<int>(req.getReqBuffer().length())){
		req.setPostType("");
		return;
	}
	std::istringstream temp(req.getReqBuffer().substr(pos, req.getReqBuffer().length() - pos));
	std::string num;
	std::getline(temp, num, '\n');
	req.setPostType(num);
	std::cout << req.getPostType() << endl;
	exit(0);
}
