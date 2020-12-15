/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:36:05 by imicah            #+#    #+#             */
/*   Updated: 2020/12/15 23:11:02 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <iostream>
#include "VirtualServer.hpp"

VirtualServer::VirtualServer() : _limit_client_body_size(0) { }

void	VirtualServer::set_ip(const std::string& ip) { _ip = ip; }
void	VirtualServer::set_port(const std::string& port) { _port = port; }
void	VirtualServer::add_server_name(const std::string& server_name) { _server_names.push_back(server_name); }
void	VirtualServer::add_location(const Location& list_locations) { _list_locations.push_back(list_locations); }

void	VirtualServer::init_socket() {
	struct sockaddr_in	sock_addr;
	int 				opt;

	ft_memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = PF_INET;
	sock_addr.sin_port = ft_htons(ft_atoi(_port.c_str()));
	sock_addr.sin_addr.s_addr = inet_addr(_ip.c_str());

	_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	setsockopt(_socket, SOL_SOCKET,  SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(_socket, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) < 0)
		throw std::exception();
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	listen(_socket, 10);
}

VirtualServer::~VirtualServer() { close(_socket); }

void	VirtualServer::set_limit_client_body_size(int limit_client_body_size) {
	_limit_client_body_size = limit_client_body_size;
}

const std::string&					VirtualServer::get_ip() const { return (_ip); }
const std::string&					VirtualServer::get_port() const { return (_port); }
const std::vector<std::string>&		VirtualServer::get_server_names() const { return (_server_names); }

const std::string&	VirtualServer::get_error_page(const std::string& status_code) const {
	try {
		return _error_pages.at(status_code);
	}
	catch (std::exception&) {
		return ("");
	}
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
	int 				priority;
	const Location*		result;

	priority = 0;
	result = nullptr;
	for (const auto& location : _list_locations)
		if (request.get_target().find(location.get_path().c_str(), 0, location.get_path().size()) == 0) {
			if (priority < location.get_path().size()) {
				priority = location.get_path().size();
				result = &location;
			}
		}
	return (*result);
}

void VirtualServer::add_error_page(const std::string&, const std::string&) { }

int VirtualServer::get_socket() const { return (_socket); }

void VirtualServer::set_socket(int socket) { _socket = socket; }
