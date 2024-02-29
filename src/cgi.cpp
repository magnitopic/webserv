/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 18:55:25 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/29 16:43:32 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
				return 1;
			}
		}
		else if (result == -1)
			return 2;
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

bool cgiForGetReq(Request &req, Response &resp, Server &server)
{
	int fds[2];
	pid_t id;
	std::string pathInfoStr = "PATH_INFO=" + req.getterGetArgs();
	std::vector<char> pathInfo(pathInfoStr.begin(), pathInfoStr.end());
	pathInfo.push_back('\0'); // don't forget the null terminator

	char pyPath[] = "usr/bin/python3";

	std::string absPath = req.getAbsPath();
	char *absPathCStr = new char[absPath.length() + 1];
	std::strcpy(absPathCStr, absPath.c_str());

	char *args[] = {pyPath, absPathCStr, NULL};
	char *env[2];

	env[0] = pathInfo.data();
	env[1] = NULL;

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
		exit(1);
	}
	// original process
	close(fds[1]);
	delete[] absPathCStr;

	int status = checkTimeoutCGI(id, fds);
	if (status == 1)
	{
		generateCGIerror(resp, server, 508);
		return false;
	}
	else if (status == 2)
	{
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

bool cgiForPostReq(PostReq& post, Request &req, Response &resp, Server &server)
{
	int fds[2];
	pid_t id;
	std::string content = "name=" + post.getFileName() + "&content=" + post.getFileContent() + "\0";

	char pyPath[] = "usr/bin/python3";

	std::string absPath = req.getAbsPath();
	char *absPathCStr = new char[absPath.length() + 1];
	std::strcpy(absPathCStr, absPath.c_str());

	char *args[] = {pyPath, absPathCStr, NULL};

	// creating child process
	if (pipe(fds) == -1)
		raiseError("CGI pipe error");
	if ((id = fork()) == -1)
		raiseError("CGI fork error");
	// child process executes the python CGI
	if (id == 0)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		execve("/usr/bin/python3", const_cast<char **>(args), NULL);
		perror("execve");
		exit(1);
	}

	while (content.size() > 0)
	{
		write(fds[1], content.c_str(), 1);
		content = content.substr(1, content.length() - 1);
	}
	write(fds[1], "\0", 1);
	// original process
	close(fds[1]);
	delete[] absPathCStr;

	int status = checkTimeoutCGI(id, fds);
	if (status == 1)
	{
		generateCGIerror(resp, server, 508);
		return false;
	}
	else if (status == 2)
	{
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
