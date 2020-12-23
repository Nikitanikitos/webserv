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
		throw std::exception();
	fcntl(server_socket, F_SETFL, O_NONBLOCK);
	listen(server_socket, 1000);
}

Location*	VirtualServer::getLocation(HttpRequest* request) const {
	for (int i = 0; i < list_locations.size(); ++i) {
		if (request->getTarget().find(list_locations[i]->getPath()) == 0)
			return (list_locations[i]);
	}
	return (0);
}

void		VirtualServer::sortServerNames() { std::sort(server_names.begin(), server_names.end()); }

bool		operator==(const VirtualServer& virtual_server_l, const VirtualServer& virtual_server_r) {
	return (virtual_server_l.getPort() == virtual_server_r.getPort() &&
			virtual_server_l.getHost() == virtual_server_r.getHost() &&
			virtual_server_l.getServerNames() == virtual_server_r.getServerNames());
}
