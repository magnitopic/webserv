/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 18:22:38 by alaparic          #+#    #+#             */
/*   Updated: 2024/03/15 15:14:23 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "../../include/webserv.hpp"

/**
 * Manages the behabour that the server will have with DELETE requests
 */
void deleteMethod(Server &server, Request &req, Response &resp)
{
	if (access(req.getAbsPath().c_str(), F_OK) == 0)
	{
		DIR *dir;
		if ((dir = opendir(req.getAbsPath().c_str())) != NULL){
			resp.setErrorCode(403);
			resp.setResponseHTML(403);
			resp.setContentLength(resp.getResponse());
			req.setContentType(parseContentType("html"));
			resp.generateHeader(403, server);
			return;
		}
		const int result = remove(req.getAbsPath().c_str());
		if (result == 0)
		{
			resp.setErrorCode(200);
			resp.setResponse("<html>\n<body>\n<h1>File deleted.</h1>\n</body>\n</html>\n");
			resp.setContentLength(resp.getResponse());
			resp.generateHeader(200, server);
			req.setContentType(parseContentType("html"));
		}
		else
		{
			resp.setErrorCode(404);
			resp.setResponseNotFound();
			resp.setContentLength(resp.getResponse());
			req.setContentType(parseContentType("html"));
			resp.generateHeader(404, server);
		}
	}
	else
	{
		resp.setErrorCode(404);
		resp.setResponseHTML(404);
		resp.setContentLength(resp.getResponse());
		req.setContentType(parseContentType("html"));
		resp.generateHeader(404, server);
	}
}