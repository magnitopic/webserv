/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 18:55:25 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/28 17:12:32 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/webserv.hpp"

static bool checkTimeoutCGI(pid_t id, int fds[2])
{
	bool cgiFinished = 0;
	pid_t result;
	time_t cgiTime = time(NULL);
	if (cgiTime == -1)
		raiseError("error getting time");

	while (!cgiFinished)
	{
		result = waitpid(id, NULL, WNOHANG);

		if (result == 0)
		{
			time_t auxTime = time(NULL);
			if (auxTime == -1)
				raiseError("error getting time");
			if ((auxTime - cgiTime) > MAX_CGI_TIME)
			{
				kill(id, SIGKILL);
				close(fds[0]);
				// TODO: http 508 error response
				std::cerr << BLUE << "==> ❌ " << RED << "CGI ERROR: Infinite loop" << RESET << std::endl;
				return false;
			}
		}
		else if (result == -1)
		{
			std::cerr << BLUE << "==> ❌ " << RED << "CGI ERROR: Error executing script" << RESET << std::endl;
		}
		else
			cgiFinished = !cgiFinished;
	}
	return true;
}

static void generateCGIresponse(Request &req, Response &resp, Server &server, std::string cgiResponse)
{
	resp.setErrorCode(200);
	resp.setResponse(cgiResponse);
	resp.setContentLength(resp.getResponse());
	req.setContentType(parseContentType("html"));
	resp.generateHeaderContent(200, req.getContentType(), server);
}

bool cgiForPostReq(Request &req, Response &resp, Server &server, e_action type)
{
	int fds[2];
	pid_t id;
	std::string pathInfo = "PATH_INFO=" + req.getterGetArgs();
	char pyPath[] = "usr/bin/python3";
	char *args[] = {pyPath, const_cast<char *>(req.getAbsPath().c_str()), NULL};
	char *env[2];

	if (type == 0)
	{
		env[0] = const_cast<char *>(pathInfo.c_str());
		env[1] = NULL;
	}

	// creating child process
	if (pipe(fds) == -1)
		raiseError("CGI pipe error");
	if ((id = fork()) == -1)
		raiseError("CGI fork error");

	// child process executes the python CGI
	if (id == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		execve("/usr/bin/python3", const_cast<char **>(args), const_cast<char **>(env));
		perror("execve");
		return false;
	}
	// original process
	close(fds[1]);

	if (!checkTimeoutCGI(id, fds))
		return false;

	char buf;
	std::string cgiResponse = "";
	while (read(fds[0], &buf, 1) > 0)
		cgiResponse += buf;

	generateCGIresponse(req, resp, server, cgiResponse);
	close(fds[0]);
	return true;
}
