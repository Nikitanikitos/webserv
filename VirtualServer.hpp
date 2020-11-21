/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:57 by nikita            #+#    #+#             */
/*   Updated: 2020/11/21 20:21:24 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_VIRTUALSERVER_HPP
# define WEBSERV_VIRTUALSERVER_HPP

# include <map>
# include "Route.hpp"
# include <list>
# include <netinet/in.h>

class VirtualServer {
private:
	typedef		std::string						_route_path;
	typedef		std::map<_route_path, Route>	_route_list_type;

	in_addr_t				_host;
	std::vector<in_port_t>	_ports;
	std::vector<int> 		_vs_socket;
	int 					_limit_client_body_size;
	std::list<std::string>	_server_name;
	std::string				_error_pages;
	_route_list_type		_routes;

public:
	VirtualServer();
	virtual ~VirtualServer();

	void	set_server_name(const std::list<std::string>&);
	void	add_port(in_port_t port);
	void	set_host(in_addr_t);
	void	add_route(const Route&);

	void	initialize_sockets();
};

#endif //WEBSERV_VIRTUALSERVER_HPP
