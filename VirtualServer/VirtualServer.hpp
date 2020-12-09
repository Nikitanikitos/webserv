/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:57 by nikita            #+#    #+#             */
/*   Updated: 2020/12/04 03:22:46 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_VIRTUALSERVER_HPP
# define WEBSERV_VIRTUALSERVER_HPP

# include <netinet/in.h>
# include <arpa/inet.h>
# include <zconf.h>
# include <fcntl.h>
# include "Request.hpp"
# include "Location.hpp"
# include "libft.hpp"

class	VirtualServer {
public:
	std::vector<int>						vs_sockets;

private:
	std::string								_host; /* конвертировать в число через функцию inet_addr */
	std::vector<std::string>				_ports;
	int										_limit_client_body_size;
	std::vector<std::string>				_server_names; /* если sever_names в конфиг файле нет, server_name = _host */
	std::map<std::string, std::string>		_error_pages;
	std::vector<Location>					_list_locations;

	void	_init_sock_addr(struct sockaddr_in&, const std::string&);

	static int		_create_socket(struct sockaddr_in&);

public:
	VirtualServer();
	/* Инициализирует путь к _error_page */

	virtual ~VirtualServer();
	/* закрытие сокетов */

	void												init_sockets();

	const std::string&									get_error_page(const std::string& status_code) const;

	/* инициализирует сокеты по хосту и портам */

	void												add_server_name(const std::string&);
	void												add_error_page(const std::string&, const std::string&);
	void												add_port(const std::string&);

	Location											get_location(const Request& request) const;

	[[nodiscard]] const std::vector<std::string>&		get_server_names() const;
	[[nodiscard]] const std::vector<std::string>&		get_ports() const;
	[[nodiscard]] const std::string&					get_host() const;

	void												add_location(const Location& list_locations);
	void												set_limit_client_body_size(int);

	void												set_host(const std::string&);

};

#endif //WEBSERV_VIRTUALSERVER_HPP
