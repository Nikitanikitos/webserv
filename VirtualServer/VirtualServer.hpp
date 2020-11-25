/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:57 by nikita            #+#    #+#             */
/*   Updated: 2020/11/25 04:21:46 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_VIRTUALSERVER_HPP
# define WEBSERV_VIRTUALSERVER_HPP

# include <netinet/in.h>
# include <arpa/inet.h>
# include <zconf.h>
# include <fcntl.h>
# include "Location.hpp"
# include "libft.hpp"

class VirtualServer {
public:
	std::vector<int>			vs_sockets;

private:
	std::string					_host; /* конвертировать в число через функцию inet_addr */
	std::vector<std::string>	_ports;
	int							_limit_client_body_size;
	std::vector<std::string>	_server_names; /* если sever_names в конфиг файле нет, server_name = _host */
	std::string					_error_pages;
	std::vector<Location>		_list_locations;

	void	_init_sock_addr(struct sockaddr_in&, const std::string&);

	int		_create_socket(struct sockaddr_in&);

public:
	VirtualServer();
	/* Инициализирует путь к _error_page */

	virtual ~VirtualServer();
	/* закрытие сокетов */

	void	add_server_name(const std::string&);
	void	add_port(const std::string&);
	void	add_route(const Location&);

	void	set_limit_client_body_size(int);
	void	set_host(const std::string&);

	void	init_sockets();
	/* инициализирует сокеты по хосту и портам */
};

#endif //WEBSERV_VIRTUALSERVER_HPP
