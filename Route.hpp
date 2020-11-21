/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 16:25:22 by imicah            #+#    #+#             */
/*   Updated: 2020/11/21 03:02:08 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_ROUTE_HPP
# define WEBSERV_ROUTE_HPP

# include <set>
# include <string>
# include <vector>
# include <stdint.h>

enum {
	_get,
	_head,
	_post,
	_put,
	_delete,
	_connect,
	_options,
	_trace,
	_patch
};

enum {
	_accepted = true,
	_non_accepted = false
};

class	Route
{
private:
	std::vector<bool>		accepted_methods;
	std::string				root;
	bool					autoindex;
	std::string				cgi_path; /* Путь к cgi клиенту, если указан в route, в противном случае NULL */

public:
	Route();
	/* инициализирует вектор доступных методов нулями, выставляет значение autoindex  */

	std::string				get_root_directory() const;
	bool					get_autoindex_value() const;
	std::set<std::string>	get_accepted_methods() const;

	void					set_accepted_method(const uint8_t);
	/* Принимает индекс метода и выставляет его значение */

	void 					set_autoindex(const bool);
	void					set_root(const std::string&);
	void					set_cgi_path(const std::string&);
};

#endif //WEBSERV_ROUTE_HPP
