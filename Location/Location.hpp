/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:29:23 by nikita            #+#    #+#             */
/*   Updated: 2020/11/25 04:49:17 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_LOCATION_HPP
#define WEBSERV_LOCATION_HPP

# include <vector>
# include <string>
# include <stdint.h>

enum {
	accepted = true,
	non_accepted = false
};

enum {
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	CONNECT,
	OPTIONS,
	TRACE,
	PATCH
};

enum {
	_default,
	cgi,
	proxy
};

class Location {
private:
	std::vector<bool>	_accepted_methods;
	std::string			_root;
	std::string			_cgi_pass;
	std::string			_proxy_pass;
	bool				_autoindex;
	std::string			_if_request_is_directory;

public:
	int 				location_type;

	Location();
	~Location() = default;

	void set_location_type(int location_type);

	void set_root(const std::string&);
	void set_if_request_is_directory(const std::string&);
	void set_cgi_path(const std::string&);
	void set_autoindex(bool);

	void add_accepted_method(uint8_t);
};

#endif //WEBSERV_LOCATION_HPP
