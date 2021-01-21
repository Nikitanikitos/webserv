/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/12/21 23:48:33 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

int		WebServ::working = 1;

void				WebServ::initSets(fd_set &writefd_set, fd_set &readfd_set, int &max_fd) {
	max_fd = virtual_servers.back()->getSocket();
	FD_ZERO(&readfd_set);
	FD_ZERO(&writefd_set);

	for (size_t i = 0; i < virtual_servers.size(); ++i)
		FD_SET(virtual_servers[i]->getSocket(), &readfd_set);
}

void				WebServ::addClientInTaskQueue(fd_set& readfd_set, fd_set& writefd_set) {
	std::vector<Client*>::iterator it;

	for (it = clients.begin(); it != clients.end(); ++it) {
		if (!(*it)->inTaskQueue() && ((*it)->getStage() == close_connection || (*it)->connectionTimedOut()))
			deleteClient(it);
		else if (!(*it)->inTaskQueue() && (FD_ISSET((*it)->getSocket(), &readfd_set) || FD_ISSET((*it)->getSocket(), &writefd_set)))
			thread_pool.pushTask(*it);
		if (clients.empty())
			break ;
	}
}

void				WebServ::addClientSocketInSet(fd_set& readfd_set, fd_set& writefd_set, int& max_fd) {
	for (size_t i = 0; i < clients.size(); ++i) {
		const int&		client_socket = clients[i]->getSocket();
		if (clients[i]->getStage() == parsing_request)
			FD_SET(client_socket, &readfd_set);
		else if (clients[i]->getStage() != parsing_request) // TODO если что то зависнит, посмотрите сюда
			FD_SET(client_socket, &writefd_set);
		max_fd = (client_socket > max_fd) ? client_socket : max_fd;
	}
}

void				WebServ::addNewClient(fd_set& readfd_set) {
	int 				client_socket;

	for (size_t i = 0; i < virtual_servers.size(); ++i) {
		if (FD_ISSET(virtual_servers[i]->getSocket(), &readfd_set)) {
			if ((client_socket = accept(virtual_servers[i]->getSocket(), 0, 0)) > 0)
				clients.push_back(new Client(client_socket, virtual_servers[i]->getHost(),
											 virtual_servers[i]->getPort()));
		}
	}
}

void				WebServ::runServer() {
	fd_set			writefd_set;
	fd_set			readfd_set;
	int 			max_fd;
	struct timeval	tv;

	createWorkers();
	tv.tv_usec = 0;
	while (WebServ::working) {
		tv.tv_sec = TimeOut;
		initSets(writefd_set, readfd_set, max_fd);
		addClientSocketInSet(readfd_set, writefd_set, max_fd);

		select(max_fd + 1, &readfd_set, &writefd_set, 0, &tv);

//		std::cout << clients.size() << std::endl;

		addNewClient(readfd_set);
		addClientInTaskQueue(readfd_set, writefd_set);
	}
}

VirtualServer*	WebServ::getVirtualServer(Client *client) const {
	VirtualServer*		default_vs = 0;
	HttpRequest*		request = client->getRequest();

	for (size_t i = 0; i < virtual_servers.size(); ++i) {
		VirtualServer*		virtual_server = virtual_servers[i];
		if (client->getHost() == virtual_server->getHost() && client->getPort() == virtual_server->getPort()) {
			if (!default_vs) default_vs = virtual_server;
			for (size_t j = 0; j < virtual_server->getServerNames().size(); ++j)
				if (request->findHeader("host") && request->getHeader("host").substr(0, request->getHeader("host").find(':')) == virtual_server->getServerNames()[j])
					return (virtual_server);
		}
	}
	return (default_vs);
}

std::string		WebServ::getPathToTarget(HttpRequest *request, Location* location) {
	std::string 	result = request->getTarget();

	result.erase(0, location->getPath().size());
	return (location->getRoot() + "/" + result);
}

void			WebServ::addVirtualServer(VirtualServer *virtual_server) {
	for (size_t i = 0; i < virtual_servers.size(); ++i) {
		if (virtual_server->getHost() == virtual_servers[i]->getHost() && virtual_server->getPort() ==
																		  virtual_servers[i]->getPort()) {
			virtual_server->setSocket(virtual_servers[i]->getSocket());
			virtual_servers.push_back(virtual_server);
			return ;
		}
	}
	virtual_server->initSocket();
	virtual_servers.push_back(virtual_server);
}

void		WebServ::deleteClient(std::vector<Client*>::iterator& client) {
	delete *client;
	clients.erase(client);
	client = clients.begin();
}

void		WebServ::setEnvForCgi(char **env, Client *client, const std::string &path_to_target) {
	HttpRequest*	request = client->getRequest();

	env[0] = strdup("AUTH_TYPE=basic");
	env[1] = strdup(("CONTENT_LENGTH=" + std::string(request->getBody().size() ? ft_itoa(request->getBody().size()) : "")).c_str()); // TODO нужно добавить рр query и отчистить память
	env[2] = strdup(("CONTENT_TYPE=" + (request->findHeader("content-type") ? request->getHeader("content-type") : "")).c_str());
	env[3] = strdup("GATEWAY_INTERFACE=cgi/1.1");
	env[4] = strdup(("PATH_INFO=" + request->getTarget()).c_str()); // TODO че за значение
	env[5] = strdup(("PATH_TRANSLATED=" + path_to_target).c_str());
	env[6] = strdup(("QUERY_STRING=" + request->getQuery()).c_str());
	env[7] = strdup("REMOTE_ADDR=");
	env[8] = strdup("REMOTE_IDENT=");
	env[9] = strdup("REMOTE_USER=");
	env[10] = strdup(("REQUEST_METHOD=" + request->getMethod()).c_str());
	env[11] = strdup(("REQUEST_URI=http://" + client->getHost() + ":" + client->getPort() + request->getTarget()).c_str());
	env[12] = strdup(("SCRIPT_NAME=" + request->getTarget()).c_str());
	env[13] = strdup(("SERVER_NAME=" + client->getHost()).c_str());
	env[14] = strdup(("SERVER_PORT=" + client->getPort()).c_str());
	env[15] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[16] = strdup("SERVER_SOFTWARE=webserv/0.1");
	env[17] = strdup("HTTP_X_SECRET_HEADER_FOR_TEST=1");
	env[18] = 0;
}

void WebServ::cgiHandler(Client *client, const std::string &path_to_target, Location *location) {
	HttpRequest*	request = client->getRequest();
	HttpResponse*	response = client->getResponse();
	int				status;
	char*			env[19];
	bytes			data;
	int				read_bytes;
	int 			fds[2];

	pipe(fds);
	std::string		extension;
	if (path_to_target.rfind('.') != (size_t)-1)
		extension.append(path_to_target.substr(path_to_target.rfind('.')));
	else
		extension.append(".bla");
	const char* fname = "static_files/file";
	int fd = open(fname, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fork() == 0) {
		close(fds[1]);
		dup2(fds[0], 0);
		close(fds[0]);
		dup2(fd, 1);
		close(fd);
		setEnvForCgi(env, client, path_to_target);
		char *argv[3] = {const_cast<char *>(location->getCgiInterpreter(extension).c_str()), const_cast<char *>(path_to_target.c_str()), 0}; // добавить путь к интепритатору
		exit(execve(argv[0], argv, env));
	}
	else {
		int		w = 1000000;
		close(fds[0]);
		if (!request->getBody().empty())
			w = write(fds[1], request->getBody().c_str(), request->getBody().size());
		close(fds[1]);
		wait(&status);

		char*	buff = new char[w + 1];
		lseek(fd, 0, 0);
		while ((read_bytes = (read(fd, buff, w))) > 0) {
			buff[read_bytes] = 0;
			data.add(buff, read_bytes);
		}
		close(fd);

		parsingCgiResponse(response, data);
		delete [] buff;
		unlink(fname);
	}
}

void	WebServ::parsingCgiResponse(HttpResponse* response, bytes& data) {
	int 	stage = 0;
	int 	pos;

	while (!data.empty()) {
		pos = data.find("\r\n");
		std::string		q = data.substr((size_t)pos).c_str();
		switch (stage) {
			case 0:
				stage++;
				if (!q.find("Status")) {
					response->setStatusCode(q.substr(q.find(' ') + 1, 3));
					break;
				}
				else
					response->setStatusCode("200");
			case 1:
				if (q.empty())
					stage++;
				else
					response->addHeader(q.substr(0, q.find(':')), q.substr(q.find(':') + 2));
				break;
			case 2:
				response->setBody(q);
		}
		(pos == -1) ? data.clear() : data.erase(pos + 2);
	}
}
