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
private:
	int										_socket;
	std::string								_ip;
	std::string								_port;
	int										_limit_client_body_size;
	std::vector<std::string>				_server_names;
	std::map<std::string, std::string>		_error_pages;
	std::vector<Location>					_list_locations;

public:
	VirtualServer();

	virtual ~VirtualServer();

	void								init_socket();

	int									get_socket() const;
	const std::string&					get_error_page(const std::string& status_code) const;
	Location							get_location(const Request& request) const;
	const std::vector<std::string>&		get_server_names() const;
	const std::string&					get_port() const;
	const std::string&					get_ip() const;

	void								add_server_name(const std::string&);
	void								add_error_page(const std::string&, const std::string&);
	void								add_location(const Location& list_locations);
	void								set_port(const std::string &port);
	void								set_limit_client_body_size(int);
	void								set_ip(const std::string& ip);
	void								set_socket(int socket);
};

#endif //WEBSERV_VIRTUALSERVER_HPP
