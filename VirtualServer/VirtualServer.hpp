/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:57 by nikita            #+#    #+#             */
/*   Updated: 2020/12/20 11:43:13 by nikita           ###   ########.fr       */
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
	typedef std::map<std::string, std::string>		error_pages_t;

	int										_socket;
	std::string								_ip;
	std::string								_port;
	int										_limit_client_body_size;
	std::vector<std::string>				_server_names;
	error_pages_t							_error_pages;
	std::vector<Location*>					_list_locations;

public:
	VirtualServer() : _limit_client_body_size(0) { }

	virtual ~VirtualServer()  {
		close(_socket);
		for (int i = 0; i < _list_locations.size(); ++i)
			delete _list_locations[i];
	}

	void								InitSocket();
	void								SortServerNames();

	int									GetSocket() const;
	bool								FindErrorPage(const std::string& error_page) const;
	const std::string&					GetErrorPage(const std::string& status_code) const;
	Location*							GetLocation(Request *request) const;
	const std::vector<std::string>&		GetServerNames() const;
	const std::string&					GetPort() const;
	const std::string&					GetIp() const;
	int									GetLimitBodySize() const;

	void								AddServerName(const std::string& server_name);
	void								AddErrorPage(const std::string& key, const std::string& value);
	void								AddLocation(Location *location);
	void								SetPort(const std::string &port);
	void								SetLimitClientBodySize(int limit_client_body_size);
	void								SetIp(const std::string& ip);
	void								SetSocket(int socket);
};

bool					operator==(const VirtualServer& virtual_server_l, const VirtualServer& virtual_server_r); // TODO доделать сравнение

#endif //WEBSERV_VIRTUALSERVER_HPP
