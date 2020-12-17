/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/12/17 17:06:48 by imicah           ###   ########.fr       */
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
	max_fd = _virtual_servers.back().GetSocket();
	FD_ZERO(&readfd_set);
	FD_ZERO(&writefd_set);

	for (int i = 0; i < _virtual_servers.size(); ++i) {
		FD_SET(_virtual_servers[i].GetSocket(), &readfd_set);
		max_fd = (_virtual_servers[i].GetSocket() > max_fd) ? _virtual_servers[i].GetSocket() : max_fd;
	}
}

void				WebServ::_AddClientInTaskQueue(fd_set &readfd_set) {
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->GetStage() == close_connection_) {
			delete *it;
			_clients.erase(it);
			it = _clients.begin();
		}
		else if (!(*it)->InTaskQueue() && FD_ISSET((*it)->GetSocket(), &readfd_set))
			_thread_pool.PushTask((*it));
		if (_clients.empty())
			return ;
	}
}

void				WebServ::_AddClientSocketInSet(fd_set &readfd_set, int &max_fd) {
	for (int i = 0; i < _clients.size(); ++i) {
		const int		client_socket = _clients[i]->GetSocket();
		if (_clients[i]->GetStage() == read_request_)
			FD_SET(client_socket, &readfd_set);
		if (client_socket > max_fd)
			max_fd = client_socket;
	}
}

void				WebServ::_AddNewClient(fd_set& readfd_set) {
	int 	new_client;

	for (int i = 0; i < _virtual_servers.size(); ++i) {
		if (FD_ISSET(_virtual_servers[i].GetSocket(), &readfd_set)) {
			if ((new_client = accept(_virtual_servers[i].GetSocket(), 0, 0)) > 0)
				_clients.push_back(new Client(new_client, _virtual_servers[i].GetIp(), _virtual_servers[i].GetPort()));
		}
	}
}

void				WebServ::RunServer() {
	_CreateWorkers();

	fd_set		writefd_set;
	fd_set		readfd_set;
	int 		max_fd;

	while (true) {
		_InitSets(writefd_set, readfd_set, max_fd);
		_AddClientSocketInSet(readfd_set, max_fd);

		select(max_fd + 1, &readfd_set, &writefd_set, 0, 0);

		_AddNewClient(readfd_set);
		_AddClientInTaskQueue(readfd_set);
	}
}

const VirtualServer&	WebServ::_GetVirtualServer(Client *client) const {
	const VirtualServer		*default_vs = NULL;
	const Request&			request = client->GetRequest();

	for (int i = 0; i < _virtual_servers.size(); ++i) {
		const VirtualServer&	virtual_server = _virtual_servers[i];
		if (client->GetIp() == virtual_server.GetIp() && client->GetPort() == virtual_server.GetPort()) {
			if (!default_vs)
				default_vs = &virtual_server;
			for (int j = 0; j < virtual_server.GetServerNames().size(); ++j)
				if (request.GetHeader("host") == virtual_server.GetServerNames()[j])
					return (virtual_server);
		}
	}
	return (*default_vs);
}

std::string		WebServ::_GetPathToTarget(const Request& request, const Location& location) {
	std::string 	result = request.GetTarget();

	result.erase(0, location.GetPath().size());
	return (location.GetRoot() + "/" + result);
}

void WebServ::AddVirtualServer(VirtualServer &virtual_server) {
	for (int i = 0; i < _virtual_servers.size(); ++i) {
		if (virtual_server.GetIp() == _virtual_servers[i].GetIp() && virtual_server.GetPort() == _virtual_servers[i].GetPort()) {
			virtual_server.SetSocket(_virtual_servers[i].GetSocket());
			_virtual_servers.push_back(virtual_server);
			return ;
		}
	}
	virtual_server.InitSocket();
	_virtual_servers.push_back(virtual_server);
}
