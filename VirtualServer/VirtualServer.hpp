/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:57 by nikita            #+#    #+#             */
/*   Updated: 2020/12/15 22:27:48 by imicah           ###   ########.fr       */
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

private:
	std::string								_host;
	std::vector<std::string>				_ports;
	int										_socket;
	int										_limit_client_body_size;
	std::vector<std::string>				_server_names;
	std::map<std::string, std::string>		_error_pages;
	std::vector<Location>					_list_locations;

	void			_init_sock_addr(struct sockaddr_in&, const std::string&);

	static int		_create_socket(struct sockaddr_in&);

public:
	VirtualServer();

	virtual ~VirtualServer();

	void								init_sockets();

	int									get_socket() const;

	const std::string&					get_error_page(const std::string& status_code) const;

	void								add_server_name(const std::string&);
	void								add_error_page(const std::string&, const std::string&);
	void								add_port(const std::string&);

	Location							get_location(const Request& request) const;

	const std::vector<std::string>&		get_server_names() const;
	const std::vector<std::string>&		get_ports() const;
	const std::string&					get_host() const;

	void								add_location(const Location& list_locations);
	void								set_limit_client_body_size(int);

	void								set_host(const std::string&);
};

#endif //WEBSERV_VIRTUALSERVER_HPP
