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

void	VirtualServer::SetIp(const std::string& ip) { _ip = ip; }
void	VirtualServer::SetPort(const std::string& port) { _port = port; }
void	VirtualServer::AddServerName(const std::string& server_name) { _server_names.push_back(server_name); }
void	VirtualServer::AddLocation(const Location& location) { _list_locations.push_back(location); }

void	VirtualServer::InitSocket() {
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

void	VirtualServer::SetLimitClientBodySize(int limit_client_body_size) {
	_limit_client_body_size = limit_client_body_size;
}

const std::string&					VirtualServer::GetIp() const { return (_ip); }
const std::string&					VirtualServer::GetPort() const { return (_port); }
const std::vector<std::string>&		VirtualServer::GetServerNames() const { return (_server_names); }

const std::string&	VirtualServer::GetErrorPage(const std::string& status_code) const {
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

Location	VirtualServer::GetLocation(const Request& request) const {
	int 				priority;
	const Location*		result;

	priority = 0;
	result = nullptr;
	for (const auto& location : _list_locations)
		if (request.GetTarget().find(location.GetPath().c_str(), 0, location.GetPath().size()) == 0) {
			if (priority < location.GetPath().size()) {
				priority = location.GetPath().size();
				result = &location;
			}
		}
	return (*result);
}

void VirtualServer::AddErrorPage(const std::string& key, const std::string& value) { }

int VirtualServer::GetSocket() const { return (_socket); }

void VirtualServer::SetSocket(int socket) { _socket = socket; }
