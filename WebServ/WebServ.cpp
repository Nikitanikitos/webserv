/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/12/18 17:37:38 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <iostream>

WebServ::WebServ(int number_of_workers) : _number_workers(number_of_workers) { }

WebServ::~WebServ() {
	for (int i = 0; i < _clients.size(); ++i)
		delete _clients[i];
};

void				WebServ::_InitSets(fd_set &writefd_set, fd_set &readfd_set, int &max_fd) {
	max_fd = _virtual_servers.back()->GetSocket();
	FD_ZERO(&readfd_set);
	FD_ZERO(&writefd_set);

	for (int i = 0; i < _virtual_servers.size(); ++i)
		FD_SET(_virtual_servers[i]->GetSocket(), &readfd_set);
}

void				WebServ::_AddClientInTaskQueue(fd_set& readfd_set, fd_set& writefd_set) {
	std::vector<Client*>::iterator it;

	for (it = _clients.begin(); it != _clients.end(); ++it) {
		if ( !(*it)->InTaskQueue() && ((*it)->GetStage() == close_connection_)) {
			delete *it;
			_clients.erase(it);
			it = _clients.begin();
		}
		else if (!(*it)->InTaskQueue() && (FD_ISSET((*it)->GetSocket(), &readfd_set) || FD_ISSET((*it)->GetSocket(), &writefd_set)))
			_thread_pool.PushTask((*it));
		if (_clients.empty())
			break ;
	}
}

void				WebServ::_AddClientSocketInSet(fd_set& readfd_set, fd_set& writefd_set, int& max_fd) {
	for (int i = 0; i < _clients.size(); ++i) {
		const int&		client_socket = _clients[i]->GetSocket();
		if (_clients[i]->GetStage() == read_request_)
			FD_SET(client_socket, &readfd_set);
		else if (_clients[i]->GetStage() == send_response_) // TODO если что то зависнит, посмотрите сюда
			FD_SET(client_socket, &writefd_set);
		max_fd = (client_socket > max_fd) ? client_socket : max_fd;
	}
}

void				WebServ::_AddNewClient(fd_set& readfd_set) {
	int 	client_socket;

	for (int i = 0; i < _virtual_servers.size(); ++i) {
		if (FD_ISSET(_virtual_servers[i]->GetSocket(), &readfd_set)) {
			while ((client_socket = accept(_virtual_servers[i]->GetSocket(), 0, 0)) > 0)
				_clients.push_back(new Client(client_socket, _virtual_servers[i]->GetIp(), _virtual_servers[i]->GetPort()));
		}
	}
}

void				WebServ::RunServer() {
	fd_set		writefd_set;
	fd_set		readfd_set;
	int 		max_fd;

	_CreateWorkers();
	while (true) {
		_InitSets(writefd_set, readfd_set, max_fd);
		_AddClientSocketInSet(readfd_set, writefd_set, max_fd);

		select(max_fd + 1, &readfd_set, &writefd_set, 0, 0);

		std::cout << _clients.size() << std::endl;

		_AddNewClient(readfd_set);
		_AddClientInTaskQueue(readfd_set, writefd_set);
	}
}

VirtualServer*	WebServ::_GetVirtualServer(Client *client) const {
	VirtualServer*		default_vs = 0;
	Request*			request = client->GetRequest();

	for (int i = 0; i < _virtual_servers.size(); ++i) {
		VirtualServer*		virtual_server = _virtual_servers[i];
		if (client->GetIp() == virtual_server->GetIp() && client->GetPort() == virtual_server->GetPort()) {
			if (!default_vs) default_vs = virtual_server;
			for (int j = 0; j < virtual_server->GetServerNames().size(); ++j)
				if (request->GetHeader("host") == virtual_server->GetServerNames()[j])
					return (virtual_server);
		}
	}
	return (default_vs);
}

std::string		WebServ::_GetPathToTarget(Request *request, Location* location) {
	std::string 	result = request->GetTarget();

	result.erase(0, location->GetPath().size());
	return (location->GetRoot() + "/" + result);
}

void WebServ::AddVirtualServer(VirtualServer *virtual_server) {
	for (int i = 0; i < _virtual_servers.size(); ++i) {
		if (virtual_server->GetIp() == _virtual_servers[i]->GetIp() && virtual_server->GetPort() == _virtual_servers[i]->GetPort()) {
			virtual_server->SetSocket(_virtual_servers[i]->GetSocket());
			_virtual_servers.push_back(virtual_server);
			return ;
		}
	}
	virtual_server->InitSocket();
	_virtual_servers.push_back(virtual_server);
}
