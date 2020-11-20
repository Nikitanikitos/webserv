/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 14:54:53 by imicah            #+#    #+#             */
/*   Updated: 2020/11/20 21:04:10 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_VIRTUALSERVER_HPP
# define WEBSERV_VIRTUALSERVER_HPP

# include <string>
# include <map>
# include <vector>
# include <list>

# include "Route.hpp"

/* vs - virtual_server */

class	VirtualServer
{
private:
	typedef		std::string					_route_path;
	typedef std::map<_route_path, Route>	_route_list_type;

	int						_host;
	std::vector<int> 		_ports;
	int 					_vs_socket;
	int 					_limit_client_body_size;
	std::list<std::string>	_server_name;
	std::string				_error_pages;
	_route_list_type		_routes;

public:
	VirtualServer(int port, int host, std::string& server_name, std::map<std::string, Route>& routes);
	/* Инициализирует атрибуты класса, получает _vs_socket, вызывая socket,
	 * делает bind к проту и хосту, вызывает listen,
	 * делает _vs_socket неблокируемым */

	~VirtualServer();
	/* закрывает _vs_socket */

	int		get_socket() const;
};

#endif //WEBSERV_VIRTUALSERVER_HPP
