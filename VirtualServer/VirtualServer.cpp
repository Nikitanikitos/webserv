/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:36:05 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:59:06 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"

void								VirtualServer::setHost(const std::string& ip) { host = ip; }
void								VirtualServer::setPort(const std::string& port_) { port = port_; }
void								VirtualServer::setSocket(int socket) { socket = socket; }
void								VirtualServer::addServerName(const std::string& server_name) { server_names.push_back(server_name); }
void								VirtualServer::addLocation(Location *location) { list_locations.push_back(location); }
void								VirtualServer::setLimitClientBodySize(int limit_client_body_size_)
	{ limit_client_body_size = limit_client_body_size_; }

void								VirtualServer::initSocket() {
	struct sockaddr_in	sock_addr;
	int 				opt = 1;

	ft_memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = PF_INET;
	sock_addr.sin_port = ft_htons(ft_atoi(port.c_str()));
	sock_addr.sin_addr.s_addr = inet_addr(host.c_str());

	server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(server_socket, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) < 0)
		throw std::exception();
	fcntl(server_socket, F_SETFL, O_NONBLOCK);
	listen(server_socket, 1000);
}

const std::string&					VirtualServer::getHost() const { return (host); }
const std::string&					VirtualServer::getPort() const { return (port); }
const std::vector<std::string>&		VirtualServer::getServerNames() const { return (server_names); }
int									VirtualServer::getSocket() const { return (server_socket); }
int									VirtualServer::getLimitBodySize() const { return (limit_client_body_size); }

Location*							VirtualServer::getLocation(HttpRequest* request) const {
	for (int i = 0; i < list_locations.size(); ++i) {
		if (request->getTarget().find(list_locations[i]->getPath()) == 0)
			return (list_locations[i]);
	}
	return (0);
}

void					VirtualServer::addErrorPage(const std::string& key, const std::string& value)
	{ error_pages.insert(std::make_pair(key, value)); }
const std::string&		VirtualServer::getErrorPage(const std::string& status_code) const
	{ return error_pages.at(status_code); }
bool					VirtualServer::findErrorPage(const std::string& error_page) const
	{ return (error_pages.count(error_page)); }

void					VirtualServer::sortServerNames() { std::sort(server_names.begin(), server_names.end()); }

bool					operator==(const VirtualServer& virtual_server_l, const VirtualServer& virtual_server_r) {
	return (virtual_server_l.getPort() == virtual_server_r.getPort() &&
			virtual_server_l.getHost() == virtual_server_r.getHost() &&
			virtual_server_l.getServerNames() == virtual_server_r.getServerNames());
}
