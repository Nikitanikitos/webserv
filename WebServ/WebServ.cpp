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
#include <iostream>

int		WebServ::working = 1;

void				WebServ::initSets(fd_set &writefd_set, fd_set &readfd_set, int &max_fd) {
	max_fd = virtual_servers.back()->getSocket();
	FD_ZERO(&readfd_set);
	FD_ZERO(&writefd_set);

	for (int i = 0; i < virtual_servers.size(); ++i)
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
	for (int i = 0; i < clients.size(); ++i) {
		const int&		client_socket = clients[i]->getSocket();
		if (clients[i]->getStage() == parsing_request)
			FD_SET(client_socket, &readfd_set);
		else if (clients[i]->getStage() != parsing_request) // TODO если что то зависнит, посмотрите сюда
			FD_SET(client_socket, &writefd_set);
		max_fd = (client_socket > max_fd) ? client_socket : max_fd;
	}
}

void				WebServ::addNewClient(fd_set& readfd_set) {
	int 	client_socket;

	for (int i = 0; i < virtual_servers.size(); ++i) {
		if (FD_ISSET(virtual_servers[i]->getSocket(), &readfd_set)) {
			if ((client_socket = accept(virtual_servers[i]->getSocket(), 0, 0)) > 0)
				clients.push_back(new Client(client_socket, virtual_servers[i]->getHost(), virtual_servers[i]->getPort()));
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

		std::cout << clients.size() << std::endl;

		addNewClient(readfd_set);
		addClientInTaskQueue(readfd_set, writefd_set);
	}
}

VirtualServer*	WebServ::getVirtualServer(Client *client) const {
	VirtualServer*		default_vs = 0;
	HttpRequest*		request = client->getRequest();

	for (int i = 0; i < virtual_servers.size(); ++i) {
		VirtualServer*		virtual_server = virtual_servers[i];
		if (client->getHost() == virtual_server->getHost() && client->getPort() == virtual_server->getPort()) {
			if (!default_vs) default_vs = virtual_server;
			for (int j = 0; j < virtual_server->getServerNames().size(); ++j)
				if (request->findHeader("host") && request->getHeader("host") == virtual_server->getServerNames()[j])
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
	for (int i = 0; i < virtual_servers.size(); ++i) {
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

void			WebServ::deleteClient(std::vector<Client*>::iterator& client) {
	delete *client;
	clients.erase(client);
	client = clients.begin();
}

void WebServ::setEnvForCgi(char **env, Client *client, const std::string &path_to_target) {
	env[0] = strdup("AUTH_TYPE=basic"); // basic

//    env[1] = strdup((std::string("QUERY_STRING=") + "first_name=Lebrus&last_name=Shupay&maths=PEZDA").c_str()); // Get все, что после знака вопроса (поле запроса)
//    env[2] = strdup("CONTENT_LENGTH=46");
//    env[3] = strdup("CONTENT_TYPE=text/html"); // пустой или от запроса (application/x-www-form-urlencoded для меня)
//    env[4] = strdup("GATEWAY_INTERFACE=cgi/1.1"); // Дефолтный
//    env[5] = strdup("PATH_INFO=/cgi_bin/youpi.bla"); // Выдергивается с URI (второй аргумент в запросе GET / 123)
//    env[6] = strdup("PATH_TRANSLATED=/Users/casubmar/school/cpp/cpp08_home/webserv/cgi_bin/cgi_tester"); // Физический адрес (путь к файлу из лакейшена, где лежит cgi)
//    env[7] = strdup("REMOTE_ADDR=178.207.154.253"); // Адрес Клиента
//    env[8] = strdup("REMOTE_IDENT="); // Имя клиента
//    env[9] = strdup("REMOTE_USER="); // То как клиент назван на сервере (аутентификация)
//    env[10] = strdup("REQUEST_METHOD=POST"); // Метод в реквесте
//    env[11] = strdup("REQUEST_URI=http://127.0.0.1:8080/cgi_bin/youpi.bla?first_name=Lebrus&last_name=Shupay&maths=PEZDA"); // строка запроса
//    env[12] = strdup("SCRIPT_NAME=cgi_tester"); // название скрипта // Путь к файлу из локейшена где лежит cgi
//    env[13] = strdup("SERVER_NAME=webserv/1.1"); // название сервера
//    env[14] = strdup("SERVER_PORT=8080"); // порт
//    env[15] = strdup("SERVER_PROTOCOL=HTTP/1.1"); // хттп протокол
//    env[16] = strdup("SERVER_SOFTWARE=web"); // название
//    env[17] = nullptr;

//	env[1] = strdup((std::string("QUERY_STRING=") + client->getRequest()->getQuery()).c_str());
//	if (client->getRequest()->getQuery()) // В случае POST и PUT размер body (из запроса)
//		env[2] = strdup((std::string("CONTENT_LENGTH=") + client->getRequest()->getQuery().size()));
//	else
//		env[2] = strdup((std::string("CONTENT_LENGTH=") + client->getRequest()->getHeader("content-length")).c_str());
    env[1] = strdup((std::string("QUERY_STRING=") + "first_name=Lebrus&last_name=Shupay&maths=PEZDA").c_str()); // Get все, что после знака вопроса (поле запроса)
    env[2] = strdup("CONTENT_LENGTH=46");
	if (client->getRequest()->findHeader("content-type"))
		env[3] = strdup((std::string("CONTENT_TYPE=") + client->getRequest()->getHeader("content-type")).c_str());
	else
		env[3] = strdup("CONTENT_TYPE=");
	env[4] = strdup("GATEWAY_INTERFACE=cgi/1.1");
	env[5] = strdup(std::string(("PATH_INFO=") + client->getRequest()->getTarget()).c_str());
	env[6] = strdup((std::string("PATH_TRANSLATED=") + path_to_target).c_str());
	env[7] = strdup("REMOTE_ADDR=178.207.154.253"); // Адрес Клиента
	env[8] = strdup("REMOTE_IDENT="); // Имя клиента
	env[9] = strdup("REMOTE_USER="); // То как клиент назван на сервере (аутентификация)
	env[10] = strdup((std::string("REQUEST_METHOD=") + client->getRequest()->getMethod()).c_str()); // Метод в реквесте
	env[11] = strdup((std::string("REQUEST_URI=http://") + client->getHost() + ":" + client->getPort() + client->getRequest()->getTarget()).c_str());
	env[12] = strdup((std::string("SCRIPT_NAME=") + client->getRequest()->getTarget()).c_str()); // название скрипта // Путь к файлу из локейшена где лежит cgi
	env[13] = strdup("SERVER_NAME=webserv/1.1"); // название сервера
	env[14] = strdup((std::string("SERVER_PORT=") + client->getPort()).c_str()); // порт
	env[15] = strdup("SERVER_PROTOCOL=HTTP/1.1"); // хттп протокол
	env[16] = strdup("SERVER_SOFTWARE=web"); // название
	env[17] = nullptr;
}

void WebServ::cgiHandler(Client *client, const std::string &path_to_target) {
	int fds[2];
	int status = 0;
	pid_t pid;
	pipe(fds);
	pid = fork();
	if (pid == 0) {
		char *env[18];
		dup2(fds[0], 0);
		dup2(fds[1], 1);
		setEnvForCgi(env, client, std::string());
		char *argv[3] = {"/usr/bin/python", const_cast<char *>(path_to_target.c_str()), 0}; // добавить путь к интепритатору
		if (client->getRequest()->findHeader("content-length"))
			write(fds[1], client->getRequest()->getBody().c_str(), client->getRequest()->getBody().size());
		int pp = execve(argv[0], argv, env);
//		std::cout << "hello" << '\n';
		exit(pp);
	} else {
		wait(&status);
		char buff[1024];
		int read_bytes;

		close(fds[1]);
		while ((read_bytes = (read(fds[0], buff, 1024))) > 0)
			client->getResponse()->addToBuffer(buff, read_bytes);
		close(fds[0]);
	}
}

