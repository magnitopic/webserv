/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 18:55:25 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/28 10:17:38 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/webserv.hpp"
#define MAX_CGI_TIME 5

void raiseError(const char *msg)
{
	std::cerr << RED << std::endl;
	perror(msg);
	std::cerr << RESET << std::endl;
	exit(1);
}

bool cgiForPostReq(char *route, char **args, char **env)
{
	int fds[2];
	pid_t id;

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
		execve("/usr/bin/python3", args, env);
		perror("execve");
		return false;
	}
	// original process
	close(fds[1]);

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
				std::cerr << BLUE << "==> ❌ " << RED << "CGI ERROR: Infinite loop" << std::endl;
				return false;
			}
		}
		else if (result == -1)
		{
			std::cerr << BLUE << "==> ❌ " << RED << "CGI ERROR: Error executing script" << std::endl;
		}
		else
			cgiFinished = !cgiFinished;
	}
	return true;
}

int main(int argc, char **argv, char **env)
{
	cgiForPostReq(argv[1], argv, env);
	return 0;
}

