/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:57 by nikita            #+#    #+#             */
/*   Updated: 2020/11/22 01:39:03 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_VIRTUALSERVER_HPP
# define WEBSERV_VIRTUALSERVER_HPP

# include <map>
# include "Route.hpp"
# include <netinet/in.h>

class VirtualServer {
private:
	typedef		std::vector<Route>				_route_list_type;

	std::string					_host; /* конвертировать в число через функцию inet_addr */
	std::vector<std::string>	_ports; /* по умолчанию 80 */
	std::vector<int>			_vs_sockets;
	int							_limit_client_body_size;
	std::vector<std::string>	_server_name; /* если sever_name в конфиг файле нет, server_name = _host */
	std::string					_error_pages;
	_route_list_type			_list_routers;

public:
	VirtualServer();
	/* Инициализирует путь к _error_page, _host по дефолту = "0.0.0.0", _port по дефолту = 80 */

	virtual ~VirtualServer();
	/* закрытие сокетов */

	void	add_server_name(const std::string&);
	void	add_port(const std::string&);
	void	add_route(const Route&);

	void	set_limit_client_body_size(int limit_client_body_size);
	void	set_host(const std::string&);

	void	initialize_sockets();
};

#endif //WEBSERV_VIRTUALSERVER_HPP
