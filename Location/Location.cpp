/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:30:05 by nikita            #+#    #+#             */
/*   Updated: 2020/11/25 17:27:27 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : _autoindex(non_accepted), _root(""), _cgi_pass(""), _proxy_pass(""),
																		_if_request_is_directory("error_page.html") {
	_allow_methods.assign(5, non_accepted);
}

void	Location::add_accepted_method(uint8_t method_index) { _allow_methods[method_index] = accepted; }
void	Location::set_root(const std::string& root) { _root = root; }
void	Location::set_autoindex(bool autoindex) { _autoindex = autoindex; }
void	Location::set_cgi_path(const std::string& cgi_path) { _cgi_pass = cgi_path; }
void	Location::set_if_request_is_directory(const std::string& if_request_is_directory) { _if_request_is_directory = if_request_is_directory; }
void	Location::set_location_type(int location_type) { _location_type = location_type; }

const std::vector<bool>&	Location::get_allow_methods() const { return (_allow_methods); }

int						Location::get_location_type() const { return (_location_type); }

const std::string&		Location::get_path() const { return (_path); }
const std::string&		Location::get_root() const { return (_root); }

bool Location::is_allow_method(const std::string& method) const {
		static std::string	methods[NUMBER_METHODS] = {"GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS"};

		for (int i = 0; i < NUMBER_METHODS; ++i) {
			if (method == methods[i])
				return (_allow_methods[i]);
		}
		return (false);
	}

bool Location::is_autoindex() const { return (_autoindex); }
