/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:57 by nikita            #+#    #+#             */
/*   Updated: 2020/11/22 16:09:39 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_VIRTUALSERVER_HPP
# define WEBSERV_VIRTUALSERVER_HPP

# include <netinet/in.h>
# include <arpa/inet.h>
# include <zconf.h>
# include "Route.hpp"
# include "libft.hpp"

class VirtualServer {
private:
	std::string					_host; /* конвертировать в число через функцию inet_addr */
	std::vector<std::string>	_ports;
	std::vector<int>			_vs_sockets;
	int							_limit_client_body_size;
	std::vector<std::string>	_server_name; /* если sever_name в конфиг файле нет, server_name = _host */
	std::string					_error_pages;
	std::vector<Route>			_list_routers;

	void	_init_sock_addr(struct sockaddr_in&, const std::string&);
	int		_create_socket(struct sockaddr_in&);

public:
	VirtualServer();
	/* Инициализирует путь к _error_page */

	virtual ~VirtualServer();
	/* закрытие сокетов */

	void	add_server_name(const std::string&);
	void	add_port(const std::string&);
	void	add_route(const Route&);

	void	set_limit_client_body_size(int);
	void	set_host(const std::string&);

	void	init_sockets();
};

#endif //WEBSERV_VIRTUALSERVER_HPP
