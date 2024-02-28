/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 18:55:25 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/28 18:36:50 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/webserv.hpp"

static int checkTimeoutCGI(pid_t id, int fds[2])
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
				int status;
				waitpid(id, &status, 0);
				close(fds[0]);
				// TODO: http 508 error response
				//std::cerr << BLUE << "==> ❌ " << RED << "CGI ERROR: Infinite loop" << RESET << std::endl;
				return 1;
			}
		}
		else if (result == -1)
		{
			return 2;
			//std::cerr << BLUE << "==> ❌ " << RED << "CGI ERROR: Error executing script" << RESET << std::endl;
		}
		else
			cgiFinished = !cgiFinished;
	}
	return 0;
}

static void generateCGIresponse(Request &req, Response &resp, Server &server, std::string cgiResponse)
{
	resp.setErrorCode(200);
	resp.setResponse(cgiResponse);
	resp.setContentLength(resp.getResponse());
	req.setContentType(parseContentType("html"));
	resp.generateHeaderContent(200, req.getContentType(), server);
}

static void generateCGIerror(Response &resp, Server &server, int code)
{
	resp.setErrorCode(code);
	resp.generateResponse(code, resp.getErrorMsg(code), server);
	resp.setContentLength(resp.getResponse());
	resp.generateHeader(code, server);
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

	int status = checkTimeoutCGI(id, fds);
	if (status == 1){
		generateCGIerror(resp, server, 508);
		return false;
	}
	else if (status == 2){
		generateCGIerror(resp, server, 500);
		return false;
	}

	char buf;
	std::string cgiResponse = "";
	while (read(fds[0], &buf, 1) > 0)
		cgiResponse += buf;

	generateCGIresponse(req, resp, server, cgiResponse);
	if (resp.getContentLength() == 0)
		generateCGIerror(resp, server, 500);
	close(fds[0]);
	return true;
}
