/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/12/12 08:51:13 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <iostream>

WebServ::WebServ(const std::vector<VirtualServer>& list_virtual_servers) : _list_virtual_servers(list_virtual_servers), _thread_pool() {
	for (int i = 0; i < _list_virtual_servers.size(); ++i) {
		std::vector<int>	sockets = _list_virtual_servers[i].vs_sockets;
		for (int j = 0; j < sockets.size(); ++j)
			_sockets.push_back(sockets[j]);
	}
	_writefd_set = (fd_set*)malloc(sizeof(fd_set));
}

WebServ::~WebServ() = default;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
void		WebServ::run_server() {
	_create_workers();

	fd_set		writefd_set;
	fd_set		readfd_set;
	int 		max_fd;

	FD_ZERO(_writefd_set);
	while (true) {
		max_fd = _sockets.back();
		FD_ZERO(&readfd_set);

		for (int i = 0; i < _sockets.size(); ++i)
			FD_SET(_sockets[i], &readfd_set);

		for (int i = 0; i < _clients.size(); ++i) {
			const int		client_socket = _clients[i]->get_socket();
			if (_clients[i]->get_stage() == read_request_)
				FD_SET(client_socket, &readfd_set);
			else if (_clients[i]->get_stage() == send_response_)
				FD_SET(client_socket, _writefd_set);
			if (client_socket > max_fd)
				max_fd = client_socket;
		}

		select(max_fd + 1, &readfd_set, _writefd_set,  nullptr, nullptr);

		for (int i = 0; i < _sockets.size(); ++i) {
			if (FD_ISSET(_sockets[i], &readfd_set)) {
				int 	new_client;
				while ((new_client = accept(_sockets[i], nullptr, nullptr)) != -1)
					_clients.push_back(new Client(new_client, read_request_));
			}
		}

		for (int i = 0; i < _clients.size(); ++i) {
			if (!_clients[i]->in_task_queue() && (_clients[i]->ready_to_action(&readfd_set, read_request_)))
				_thread_pool.push_task(_clients[i]);
		}
	}
}

const VirtualServer&	WebServ::_get_virtual_server(Request *request) const {
	bool					default_vs_flag;
	const VirtualServer		*default_vs;

	default_vs_flag = false;
	for (int i = 0; i < _list_virtual_servers.size(); ++i) {
		std::vector<std::string>	ports = _list_virtual_servers[i].get_ports();
		for (int j = 0; j < ports.size(); ++j) {
			if (request->get_port() == ports[j]) {
				if (!default_vs_flag) {
					default_vs = &_list_virtual_servers[i];
					default_vs_flag = true;
				}
				for (const auto& server_name : _list_virtual_servers[i].get_server_names())
					if (request->get_header(HOST) == server_name) {
						default_vs = &_list_virtual_servers[i];
						break ;
					}
			}
		}
	}
	return (*default_vs);
}

std::string		WebServ::_get_path_to_target(const Request& request, const Location& location) {
	std::string 	result = request.get_target().substr(0, location.get_path().size());

	if (result.empty()) {
		if (int fd = open(location.get_index().c_str(), O_RDONLY) > 0) {
			close(fd);
			result.append(location.get_index());
		}
		else
			result.append(".");
	}
	return (result);
}
