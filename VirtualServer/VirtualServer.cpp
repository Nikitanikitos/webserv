/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:36:05 by imicah            #+#    #+#             */
/*   Updated: 2020/12/04 00:38:09 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"

VirtualServer::VirtualServer() : _limit_client_body_size(0) { }

void	VirtualServer::set_host(const std::string& host) { _host = host; }

void	VirtualServer::add_port(const std::string& port) { _ports.push_back(port); }
void	VirtualServer::add_server_name(const std::string& server_name) { _server_names.push_back(server_name); }
void	VirtualServer::add_route(const Location& route) { _list_locations.push_back(route); }

void	VirtualServer::init_sockets() {
	for (const std::string& item : _ports) {
		struct sockaddr_in		sock_addr;

		ft_memset(&sock_addr, 0, sizeof(sock_addr));
		_init_sock_addr(sock_addr, item);
		vs_sockets.push_back(_create_socket(sock_addr));
	}
}

VirtualServer::~VirtualServer() {
	for (int vsSocket : vs_sockets)
		close(vsSocket);
}

void	VirtualServer::set_limit_client_body_size(int limit_client_body_size) {
	_limit_client_body_size = limit_client_body_size;
}

void	VirtualServer::_init_sock_addr(sockaddr_in& sock_addr, const std::string& item) {
	sock_addr.sin_family = PF_INET;
	sock_addr.sin_port = ft_htons(ft_atoi(item.c_str()));
	sock_addr.sin_addr.s_addr = ft_htonl(inet_addr(_host.c_str()));
}

int		VirtualServer::_create_socket(sockaddr_in& sock_addr) {
	int		fd_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (bind(fd_socket, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) < 0)
		throw std::exception();
	fcntl(fd_socket, F_SETFL, O_NONBLOCK);
	listen(fd_socket, 10);
	return (fd_socket);
}

const std::string&					VirtualServer::get_host() const { return (_host); }
const std::vector<std::string>&		VirtualServer::get_ports() const { return (_ports); }
const std::vector<std::string>&		VirtualServer::get_server_names() const { return (_server_names); }

const std::string&	VirtualServer::get_error_page(const std::string& status_code) const {
	return _error_pages.at(status_code);
}

int 		priority_compare(const std::string &string1, const std::string& string2) {
	int 	result;

	result = 0;
	if (string1.length() > string2.length())
		return (result);
	for (size_t	i = 0; i < string1.length() && i < string2.length(); ++i) {
		if (string1[i] != string2[i])
			return (0);
		result++;
	}
	return (result);
}

Location	VirtualServer::get_location(const Request& request) const {
	int 			match_priority;
	int 			compare;
	const Location* current_location;

	current_location = nullptr;
	match_priority = 0;
	for (const auto& location : _list_locations) {
		if (location.get_path() == request.get_target())
			return (location);
		compare = priority_compare(location.get_path(), request.get_target());
		if (compare > match_priority) {
			match_priority = compare;
			current_location =  &location;
		}
	}
	return (*current_location);
}
