/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 18:55:25 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/26 19:44:46 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/webserv.hpp"

bool runPythonScript(char *route, char **args, char **env)
{
	int fds[2];
	pid_t id;

	if (pipe(fds) == -1)
		raiseError("CGI pipe error");
	if (id = fork() == -1)
		raiseError("CGI fork error");

	// child process executes the python CGI
	if (id == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		execve(route, args ,env);
		perror("execve");
		return 1;
	}

	close (fds[1]);
	
}
