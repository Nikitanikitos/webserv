/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_cgi.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 15:13:01 by imicah            #+#    #+#             */
/*   Updated: 2021/01/26 19:49:51 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
# include "WebServ.hpp"

static void		setEnvForCgi(char **env, Client *client, const std::string &path_to_target) {
	HttpRequest*	request = client->getRequest();
	char*			body_size;

	body_size = (request->getQuery().empty()) ? ft_itoa(request->getBody().size()) : ft_itoa(request->getQuery().size()); //TODO вроде поправил
	env[0] = ft_strdup("AUTH_TYPE=basic");
	env[1] = ft_strdup(("CONTENT_LENGTH=" + std::string(body_size)).c_str());
	env[2] = ft_strdup(("CONTENT_TYPE=" + (request->findHeader("content-type") ? request->getHeader("content-type") : "")).c_str());
	env[3] = ft_strdup("GATEWAY_INTERFACE=cgi/1.1");
	env[4] = ft_strdup(("PATH_INFO=" + request->getTarget()).c_str());
	env[5] = ft_strdup(("PATH_TRANSLATED=" + path_to_target).c_str());
	env[6] = ft_strdup(("QUERY_STRING=" + request->getQuery()).c_str());
	env[7] = ft_strdup("REMOTE_ADDR=");
	env[8] = ft_strdup("REMOTE_IDENT=");
	env[9] = ft_strdup("REMOTE_USER=");
	env[10] = ft_strdup(("REQUEST_METHOD=" + request->getMethod()).c_str());
	env[11] = ft_strdup(("REQUEST_URI=http://" + client->getHost() + ":" + client->getPort() + request->getTarget()).c_str());
	env[12] = ft_strdup(("SCRIPT_NAME=" + request->getTarget()).c_str());
	env[13] = ft_strdup(("SERVER_NAME=" + client->getHost()).c_str());
	env[14] = ft_strdup(("SERVER_PORT=" + client->getPort()).c_str());
	env[15] = ft_strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[16] = ft_strdup("SERVER_SOFTWARE=webserv/0.1");
	env[17] = ft_strdup("HTTP_X_SECRET_HEADER_FOR_TEST=1");
	env[18] = 0;
	delete []body_size;
}

static void		run_cgi(int* pipe_fd, int file_fd, const std::string& path_to_target, Client* client, Location* location) {
	std::string		extension;
	char*			env[19];

	if (path_to_target.rfind('.') != (size_t)-1)
		extension.append(path_to_target.substr(path_to_target.rfind('.')));
	else
		extension.append(".bla");
	close(pipe_fd[1]);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	dup2(file_fd, 1);
	close(file_fd);
	setEnvForCgi(env, client, path_to_target);
	char *argv[3] = {const_cast<char *>(location->getCgiInterpreter(extension).c_str()), const_cast<char *>(path_to_target.c_str()), 0};
	exit(execve(argv[0], argv, env));
}

static bytes	send_read_in_cgi(int* pipe_fd, int file_fd, HttpRequest* request) {
	int		buffer_size;
	int		read_bytes;
	int 	status;
	char*	buff;
	bytes	data;

	if (!request->getBody().empty())
		buffer_size = write(pipe_fd[1], request->getBody().c_str(), request->getBody().size());
	else
		buffer_size = 2048;
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait(&status);

	if (!status) {
		lseek(file_fd, 0, 0);
		buff = new char[buffer_size + 1];
		while ((read_bytes = (read(file_fd, buff, buffer_size))) > 0) {
			buff[read_bytes] = 0;
			data.add(buff, read_bytes);
		}
		close(file_fd);
		delete [] buff;
	}
	else
		data.add("Status: 500\r\n");
	return (data);
}

void			WebServ::cgiHandler(Client *client, const std::string &path_to_target, Location *location) {
	const char*		file_for_answer_cgi = "RmlsZSBieSBjZ2k=";
	const int		file_fd = open(file_for_answer_cgi, O_CREAT | O_RDWR | O_TRUNC, 0666);
	HttpRequest*	request = client->getRequest();
	bytes			data;
	pid_t			pid;
	int 			pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		client->getResponse()->setStatusCode("500");
	else if ((pid = fork()) == 0)
		run_cgi(pipe_fd, file_fd, path_to_target, client, location);
	else if (pid == -1)
		client->getResponse()->setStatusCode("500");
	else {
		data = send_read_in_cgi(pipe_fd, file_fd, request);
		parsingCgiResponse(client->getResponse(), data);
	}
	unlink(file_for_answer_cgi);
}
