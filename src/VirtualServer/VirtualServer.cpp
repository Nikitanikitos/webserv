/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:36:05 by imicah            #+#    #+#             */
/*   Updated: 2021/01/27 01:00:24 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"

void		VirtualServer::initSocket() {
	struct sockaddr_in	sock_addr;
	int 				opt = 1;

	ft_memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = PF_INET;
	sock_addr.sin_port = ft_htons(ft_atoi(port.c_str()));
	sock_addr.sin_addr.s_addr = inet_addr(host.c_str());

	server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(server_socket, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) < 0)
		throw "ports are busy";
	fcntl(server_socket, F_SETFL, O_NONBLOCK);
	listen(server_socket, 1000);
}

Location*	VirtualServer::getLocation(HttpRequest* request) const {
	Location*	location = 0;
	size_t		current_size = 0;

	for (size_t i = 0; i < locations.size(); ++i) {
		if (request->getTarget().find(locations[i]->getPath()) == 0 && current_size < locations[i]->getPath().size()) {
			location = locations[i];
			current_size = locations[i]->getPath().size();
		}
	}
	return (location);
}

void		VirtualServer::sortServerNames() { std::sort(server_names.begin(), server_names.end()); }

bool		operator==(const VirtualServer& virtual_server_l, const VirtualServer& virtual_server_r) {
	if (virtual_server_l.getPort() == virtual_server_r.getPort() && virtual_server_l.getHost() == virtual_server_r.getHost()) {
		if (virtual_server_l.getServerNames().empty() && virtual_server_r.getServerNames().empty())
			return (true);
		for (size_t i = 0; i < virtual_server_l.getServerNames().size(); ++i) {
			for (size_t j = 0; j < virtual_server_r.getServerNames().size(); ++j) {
				if (virtual_server_l.getServerNames()[i] == virtual_server_r.getServerNames()[j]) return (true);
			}
		}
		return (false);
	}
	return (false);
}
