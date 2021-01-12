/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:57 by nikita            #+#    #+#             */
/*   Updated: 2020/12/21 16:12:29 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_VIRTUALSERVER_HPP
# define WEBSERV_VIRTUALSERVER_HPP

# include "libft.hpp"

# include <zconf.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>

# include <map>
# include <vector>
# include "Location.hpp"
# include "HttpRequest.hpp"
# include <algorithm>

class	VirtualServer {
private:
	typedef std::map<std::string, std::string>		error_pages_t;

	int										server_socket;
	std::string								host;
	std::string								port;
	std::vector<std::string>				server_names;
	error_pages_t							error_pages;
	std::vector<Location*>					locations;

public:
	VirtualServer() { }

	virtual ~VirtualServer()  {
		close(server_socket);
		for (int i = 0; i < locations.size(); ++i)
			delete locations[i];
	}

	void								initSocket();
	void								sortServerNames();

	Location*							getLocation(HttpRequest *request) const;
	inline int							getSocket() const { return (server_socket); }
	inline const std::string&			getPort() const { return (port); }
	inline const std::string&			getHost() const { return (host); }

	inline const std::vector<std::string>&		getServerNames() const  { return (server_names); }

	inline const std::string&			getErrorPage(const std::string& status_code) const
		{ return error_pages.at(status_code); }
	inline void							addServerName(const std::string& server_name)
		{ server_names.push_back(server_name); }
	inline void							addErrorPage(const std::string& key, const std::string& value)
		{ error_pages.insert(std::make_pair(key, value)); }
	inline void							addLocation(Location *location)
		{ locations.push_back(location); }
	inline bool							findErrorPage(const std::string& error_page) const
	{ return (error_pages.count(error_page)); }

	inline void							setPort(const std::string &port_) { port = port_; }
	inline void							setHost(const std::string& ip) { host = ip; }
	inline void							setSocket(int socket) { socket = socket; }
};

bool					operator==(const VirtualServer& virtual_server_l, const VirtualServer& virtual_server_r); // TODO доделать сравнение

#endif //WEBSERV_VIRTUALSERVER_HPP
