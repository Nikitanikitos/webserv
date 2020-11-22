/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:29:23 by nikita            #+#    #+#             */
/*   Updated: 2020/11/21 20:38:35 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_ROUTE_HPP
#define WEBSERV_ROUTE_HPP

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

class Route {
private:
	std::vector<bool> _accepted_methods;
	std::string       _root;
	bool              _autoindex;
	std::string       _cgi_path; /* Путь к cgi клиенту, если указан в route, в противном случае NULL */

public:
	Route();
	~Route() = default;

	void set_root(const std::string&);
	void set_cgi_path(const std::string&);
	void set_autoindex(bool);

	void add_accepted_method(uint8_t);
};

#endif //WEBSERV_ROUTE_HPP
