/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:36:05 by imicah            #+#    #+#             */
/*   Updated: 2020/11/22 16:06:40 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"

VirtualServer::VirtualServer() : _limit_client_body_size(0), _error_pages("error_page.html") { }

void VirtualServer::set_host(const std::string& host) {
	_host = host;
}

void VirtualServer::add_port(const std::string& port) {
	_ports.push_back(port);
}

void VirtualServer::add_server_name(const std::string& server_name) {
	_server_name.push_back(server_name);
}

void VirtualServer::add_route(const Route& route) {
	_list_routers.push_back(route);
}

void VirtualServer::init_sockets() {
	for (const std::string& item : _ports) {
		struct sockaddr_in		sock_addr;

		ft_memset(&sock_addr, 0, sizeof(sock_addr));
		_init_sock_addr(sock_addr, item);
		_vs_sockets.push_back(_create_socket(sock_addr));
	}
}

VirtualServer::~VirtualServer() {
	for (int vsSocket : _vs_sockets)
		close(vsSocket);
}

void VirtualServer::set_limit_client_body_size(int limit_client_body_size) {
	_limit_client_body_size = limit_client_body_size;
}

void VirtualServer::_init_sock_addr(sockaddr_in& sock_addr, const std::string& item) {
	sock_addr.sin_family = PF_INET;
	sock_addr.sin_port = ft_htons(ft_atoi(item.c_str()));
	sock_addr.sin_addr.s_addr = ft_htonl(inet_addr(_host.c_str()));
}

int VirtualServer::_create_socket(sockaddr_in& sock_addr) {
	int		fd_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (bind(fd_socket, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) < 0)
		throw std::exception();
	return (fd_socket);
}
