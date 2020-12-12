/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/12/12 08:07:02 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

WebServ::WebServ(const std::vector<VirtualServer>& list_virtual_servers) : _list_virtual_servers(list_virtual_servers), _thread_pool() {
	for (const VirtualServer& server : _list_virtual_servers)
		for (const int fd_socket : server.vs_sockets)
			_sockets.push_back(fd_socket);
}

WebServ::~WebServ() = default;

void		WebServ::run_server() {
	_create_workers();

	fd_set		writefd_set;
	fd_set		readfd_set;
	int 		max_fd;

	while (true) {
		max_fd = _sockets.back();
		FD_ZERO(&writefd_set);
		FD_ZERO(&readfd_set);

		for (const auto& socket : _sockets)
			FD_SET(socket, &readfd_set);

		for (const auto& client : _clients) {
			const int		client_socket = client->get_socket();
			client->lock_stage_mutex();
			if (client->get_stage() == read_request_)
				FD_SET(client_socket, &readfd_set);
			else if (client->get_stage() == send_response_)
				FD_SET(client_socket, &writefd_set);
			client->unlock_stage_mutex();
			if (client_socket > max_fd)
				max_fd = client_socket;
		}

		select(max_fd + 1, &readfd_set, &writefd_set,  nullptr, nullptr);

		for (const auto& socket : _sockets) {
			if (FD_ISSET(socket, &readfd_set)) {
				int 	new_client;
				while ((new_client = accept(socket, nullptr, nullptr)) != -1)
					_clients.push_back(new Client(new_client, read_request_));
			}
		}

		for (const auto& client : _clients) {
			if (!client->in_task_queue() && (client->ready_to_action(&readfd_set, read_request_)))
				_thread_pool.push_task(client);
		}
	}
}

const VirtualServer& WebServ::_get_virtual_server(Request *request) const {
	bool					default_vs_flag;
	const VirtualServer		*default_vs;

	default_vs_flag = false;
	for (const auto & server : _list_virtual_servers) {
		for (const auto& port : server.get_ports()) {
			if (request->get_port() == port) {
				if (!default_vs_flag) {
					default_vs = &server;
					default_vs_flag = true;
				}
				for (const auto& server_name : server.get_server_names())
					if (request->get_header(HOST) == server_name) {
						default_vs = &server;
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
