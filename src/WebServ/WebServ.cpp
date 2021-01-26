/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2021/01/27 00:26:32 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "WebServ.hpp"

int		WebServ::working = 1;
int		WebServ::imaginary_pipe[2];

void				WebServ::initSets(fd_set &writefd_set, fd_set &readfd_set, int &max_fd) {
	max_fd = virtual_servers.back()->getSocket();
	FD_ZERO(&readfd_set);
	FD_ZERO(&writefd_set);

	FD_SET(imaginary_pipe[0], &readfd_set);
	for (size_t i = 0; i < virtual_servers.size(); ++i)
		FD_SET(virtual_servers[i]->getSocket(), &readfd_set);
}

void				WebServ::addClientInTaskQueue(fd_set& readfd_set, fd_set& writefd_set) {
	std::vector<Client*>::iterator	it;
	char							buff[1];

	(FD_ISSET(imaginary_pipe[0], &readfd_set)) ? read(imaginary_pipe[0], buff, 1) : 0;
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
		else if (clients[i]->getStage() != parsing_request)
			FD_SET(client_socket, &writefd_set);
		max_fd = (client_socket > max_fd) ? client_socket : max_fd;
	}
}

void				WebServ::addNewClient(fd_set& readfd_set) {
	int 				client_socket;

	for (size_t i = 0; i < virtual_servers.size(); ++i) {
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

		addNewClient(readfd_set);
		addClientInTaskQueue(readfd_set, writefd_set);
	}
}

VirtualServer*		WebServ::getVirtualServer(Client *client) const {
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

std::string			WebServ::getPathToTarget(HttpRequest *request, Location* location) {
	std::string 	result = request->getTarget();

	result.erase(0, location->getPath().size());
	return (location->getRoot() + "/" + result);
}

void				WebServ::addVirtualServer(VirtualServer *virtual_server) {
	for (size_t i = 0; i < virtual_servers.size(); ++i) {
		if (virtual_server->getHost() == virtual_servers[i]->getHost() &&
														virtual_server->getPort() == virtual_servers[i]->getPort()) {
			virtual_server->setSocket(virtual_servers[i]->getSocket());
			virtual_servers.push_back(virtual_server);
			return ;
		}
	}
	virtual_server->initSocket();
	virtual_servers.push_back(virtual_server);
}

void				WebServ::deleteClient(std::vector<Client*>::iterator& client) {
	delete *client;
	clients.erase(client);
	client = clients.begin();
}
