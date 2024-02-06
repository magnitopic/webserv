/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentType.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 08:03:09 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/06 15:08:59 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

std::string getContentType(std::string extension)
{
	std::string::size_type pos = extension.find('.');
	
	if (pos == std::string::npos)
		return "text/plain";
	extension = extension.substr(pos + 1);
	std::map<std::string, std::string> contentTypes;
	contentTypes["txt"] = "text/plain";
	contentTypes["html"] = "text/html";
	contentTypes["css"] = "text/css";
	contentTypes["js"] = "application/javascript";
	contentTypes["json"] = "application/json";
	contentTypes["jpg"] = "image/jpeg";
	contentTypes["jpeg"] = "image/jpeg";
	contentTypes["png"] = "image/png";
	contentTypes["gif"] = "image/gif";
	contentTypes["svg"] = "image/svg+xml";
	contentTypes["mp3"] = "audio/mp3";
	contentTypes["mp4"] = "video/mp4";
	contentTypes["xml"] = "application/xml";
	contentTypes["pdf"] = "application/pdf";
	contentTypes["zip"] = "application/zip";
	contentTypes["ico"] = "image/x-icon";

	std::string contentType = contentTypes.find(extension)->second;
	return contentType;
}
