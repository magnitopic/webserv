/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentType.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 08:03:09 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/07 16:22:07 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

std::string parseContentType(std::string extension)
{
	std::map<std::string, std::string> contentTypes;
	if (extension.back() == '/')
		extension.erase(extension.size() - 1);
	contentTypes.insert(std::pair<std::string, std::string>("txt", "text/plain"));
	contentTypes.insert(std::pair<std::string, std::string>("html", "text/html"));
	contentTypes.insert(std::pair<std::string, std::string>("css", "text/css"));
	contentTypes.insert(std::pair<std::string, std::string>("js", "application/javascript"));
	contentTypes.insert(std::pair<std::string, std::string>("json", "application/json"));
	contentTypes.insert(std::pair<std::string, std::string>("jpg", "image/jpg"));
	contentTypes.insert(std::pair<std::string, std::string>("jpeg", "image/jpeg"));
	contentTypes.insert(std::pair<std::string, std::string>("png", "image/png"));
	contentTypes.insert(std::pair<std::string, std::string>("gif", "image/gif"));
	contentTypes.insert(std::pair<std::string, std::string>("svg", "image/svg+xml"));
	contentTypes.insert(std::pair<std::string, std::string>("mp3", "audio/mp3"));
	contentTypes.insert(std::pair<std::string, std::string>("mp4", "audio/mp4"));
	contentTypes.insert(std::pair<std::string, std::string>("xml", "application/xml"));
	contentTypes.insert(std::pair<std::string, std::string>("pdf", "application/pdf"));
	contentTypes.insert(std::pair<std::string, std::string>("zip", "application/zip"));
	contentTypes.insert(std::pair<std::string, std::string>("ico", "image/x-icon"));

	std::map<std::string, std::string>::iterator it = contentTypes.find(extension);
	if (it == contentTypes.end())
		return "text/plain";
	std::string contentType = contentTypes.find(extension)->second;
	return contentType;
}
