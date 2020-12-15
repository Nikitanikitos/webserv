/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/12/15 22:27:48 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <iostream>

WebServ::WebServ(int number_of_workers) : _number_workers(number_of_workers) { }

WebServ::~WebServ() {
	for (int i = 0; i < _clients.size(); ++i)
		delete _clients[i];
};

void WebServ::_init_sets(fd_set &writefd_set, fd_set &readfd_set, int &max_fd) {
	max_fd = _virtual_servers.back().get_socket();
	FD_ZERO(&readfd_set);
	FD_ZERO(&writefd_set);

	for (int i = 0; i < _virtual_servers.size(); ++i)
		FD_SET(_virtual_servers[i].get_socket(), &readfd_set);
}

void	WebServ::_add_client_in_task_queue(fd_set &readfd_set) {
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->get_stage() == close_connection_) {
			delete *it;
			_clients.erase(it);
		}
		else if (!(*it)->in_task_queue() && FD_ISSET((*it)->get_socket(), &readfd_set))
			_thread_pool.push_task((*it));
	}
}

void	WebServ::_add_client_socket_in_set(fd_set &readfd_set, int &max_fd) {
	for (int i = 0; i < _clients.size(); ++i) {
		const int		client_socket = _clients[i]->get_socket();
		if (_clients[i]->get_stage() == read_request_)
			FD_SET(client_socket, &readfd_set);
		if (client_socket > max_fd)
			max_fd = client_socket;
	}
}

void	WebServ::_add_new_client(fd_set& readfd_set) {
	int 	new_client;

	for (int i = 0; i < _virtual_servers.size(); ++i) {
		struct sockaddr_in		sock_addr;

		ft_memset(&sock_addr, 0, sizeof(sock_addr));
		if (FD_ISSET(_virtual_servers[i].get_socket(), &readfd_set)) {
			while ((new_client = accept(_virtual_servers[i].get_socket(), NULL, NULL) != -1))
				_clients.push_back(new Client(new_client, read_request_));
		}
	}
}

void		WebServ::run_server() {
	_create_workers();

	fd_set		writefd_set;
	fd_set		readfd_set;
	int 		max_fd;

	while (true) {
		_init_sets(writefd_set, readfd_set, max_fd);
		_add_client_socket_in_set(readfd_set, max_fd);

		select(max_fd + 1, &readfd_set, &writefd_set,  nullptr, nullptr);

		_add_new_client(readfd_set);
		_add_client_in_task_queue(readfd_set);
	}
}

const VirtualServer&	WebServ::_get_virtual_server(Request& request) const {
	bool					default_vs_flag;
	const VirtualServer		*default_vs;

	default_vs_flag = false;
	for (int i = 0; i < _virtual_servers.size(); ++i) {
		const std::string&	port = _virtual_servers[i].get_port();
//		for (int j = 0; j < ports.size(); ++j) {
//			if (request.get_port() == ports[j]) {
//				if (!default_vs_flag) {
//					default_vs = &_virtual_servers[i];
//					default_vs_flag = true;
//				}
//				for (const auto& server_name : _virtual_servers[i].get_server_names())
//					if (request.get_header(HOST) == server_name) {
//						default_vs = &_virtual_servers[i];
//						break ;
//					}
//			}
//		}
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

void WebServ::add_virtual_server(VirtualServer &virtual_server) {
	for (int i = 0; i < _virtual_servers.size(); ++i) {
		if (virtual_server.get_ip() == _virtual_servers[i].get_ip() && virtual_server.get_port() == _virtual_servers[i].get_port()) {
			virtual_server.set_socket(_virtual_servers[i].get_socket());
			_virtual_servers.push_back(virtual_server);
			return ;
		}
	}
	virtual_server.init_socket();
	_virtual_servers.push_back(virtual_server);
}
