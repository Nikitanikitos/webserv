/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/12/06 15:03:52 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "WebServ.hpp"

WebServ::WebServ(const std::vector<VirtualServer>& list_virtual_servers) : _list_virtual_servers(list_virtual_servers), _thread_pool() {
	FD_ZERO(&_set_of_vs_sockets);

	for (const VirtualServer& server : _list_virtual_servers)
		for (const int fd_socket : server.vs_sockets) {
			_vs_sockets.push_back(fd_socket);
			FD_SET(fd_socket, &_set_of_vs_sockets);
		}
}

WebServ::~WebServ() = default;

void		WebServ::run_server() {
	_create_workers();

	while (true) {
		select(_vs_sockets.back() + 1, &_set_of_vs_sockets, nullptr, nullptr, nullptr);
		_get_accept_from_ready_sockets();
	}
}

void WebServ::generate_request(HttpObject *http_object) {
	std::cout << "generate request " << http_object->get_client_socket() << " client" << std::endl;
}

void WebServ::generate_response(HttpObject *http_object) {
	std::cout << "generate response " << http_object->get_client_socket() << " client" << std::endl;
	try {
		const VirtualServer&	virtual_server = _get_virtual_server(http_object->get_request());
		const Location&			location = virtual_server.get_location(http_object->get_request());

		switch (location.get_location_type()) {
			case _default:
				_default_handler(http_object, virtual_server, location);
//			case cgi:
//				_cgi_handler(request, virtual_server, location, client_socket);
//			case proxy:
//				_proxy_handler(request, virtual_server, location, client_socket);
		}
	}
	catch (Request301Redirect& redirect_301) {
		http_object->set_response(redirect_301);
	}
	catch (RequestException& request_error) {
		http_object->set_response(request_error);
	}
}

void WebServ::send_response(HttpObject *http_object) {
	http_object->get_response().send_response(http_object->get_client_socket());
	close(http_object->get_client_socket());
}

const VirtualServer& WebServ::_get_virtual_server(const Request& request) const {
	bool					default_vs_flag;
	const VirtualServer		*default_vs;

	default_vs_flag = false;
	for (const auto & server : _list_virtual_servers) {
		for (const auto& port : server.get_ports()) {
			if (request.get_port() == port) {
				if (!default_vs_flag) {
					default_vs = &server;
					default_vs_flag = true;
				}
				for (const auto& server_name : server.get_server_names())
					if (request.get_header(HOST) == server_name) {
						default_vs = &server;
						break ;
					}
			}
		}
	}
	return (*default_vs);
}

std::string		WebServ::_get_path_to_target(const Request& request, const Location& location) {
	if (request.get_target() == location.get_path())
		return (location.get_root() + location.get_index());
	else
		return (location.get_root() + request.get_target().substr(location.get_path().length()));
}
