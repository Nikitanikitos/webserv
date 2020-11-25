/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:30:05 by nikita            #+#    #+#             */
/*   Updated: 2020/11/25 04:21:46 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : _autoindex(non_accepted), _root(""), _cgi_pass(""), _proxy_pass(""),
																		_if_request_is_directory("error_page.html") {
	_accepted_methods.assign(5, non_accepted);
}

void Location::set_root(const std::string& root) { _root = root; }

void Location::set_autoindex(bool autoindex) { _autoindex = autoindex; }

void Location::set_cgi_path(const std::string& cgi_path) { _cgi_pass = cgi_path; }

void Location::add_accepted_method(uint8_t method_index) { _accepted_methods[method_index] = accepted; }

void Location::set_if_request_is_directory(const std::string& if_request_is_directory) {
	_if_request_is_directory = if_request_is_directory;
}

void Location::set_location_type(int location_type) {
	_location_type = location_type;
}
