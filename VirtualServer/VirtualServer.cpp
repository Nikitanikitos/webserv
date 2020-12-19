/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:36:05 by imicah            #+#    #+#             */
/*   Updated: 2020/12/19 23:18:39 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "VirtualServer.hpp"

VirtualServer::VirtualServer() : _limit_client_body_size(0) { }

void	VirtualServer::SetIp(const std::string& ip) { _ip = ip; }
void	VirtualServer::SetPort(const std::string& port) { _port = port; }
void	VirtualServer::AddServerName(const std::string& server_name) { _server_names.push_back(server_name); }
void	VirtualServer::AddLocation(Location *location) { _list_locations.push_back(location); }
void	VirtualServer::SetLimitClientBodySize(int limit_client_body_size)
	{ _limit_client_body_size = limit_client_body_size; }

void	VirtualServer::InitSocket() {
	struct sockaddr_in	sock_addr;
	int 				opt = 1;

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

const std::string&					VirtualServer::GetIp() const { return (_ip); }
const std::string&					VirtualServer::GetPort() const { return (_port); }
const std::vector<std::string>&		VirtualServer::GetServerNames() const { return (_server_names); }

Location*							VirtualServer::GetLocation(Request* request) const {
	for (int i = 0; i < _list_locations.size(); ++i) {
		if (request->GetTarget().find(_list_locations[i]->GetPath()) == 0)
			return (_list_locations[i]);
	}
	return (0);
}

void					VirtualServer::AddErrorPage(const std::string& key, const std::string& value)
	{ _error_pages.insert(std::make_pair(key, value)); }
const std::string&		VirtualServer::GetErrorPage(const std::string& status_code) const
	{ return _error_pages.at(status_code); }
bool					VirtualServer::FindErrorPage(const std::string& error_page) const
	{ return (_error_pages.count(error_page)); }

void					VirtualServer::SetSocket(int socket) { _socket = socket; }
int						VirtualServer::GetSocket() const { return (_socket); }
void					VirtualServer::SortServerNames() { std::sort(_server_names.begin(), _server_names.end()); }

bool					operator==(const VirtualServer& virtual_server_l, const VirtualServer& virtual_server_r) {
	return (virtual_server_l.GetPort() == virtual_server_r.GetPort() &&
			virtual_server_l.GetIp() == virtual_server_r.GetIp() &&
			virtual_server_l.GetServerNames() == virtual_server_r.GetServerNames());
}
