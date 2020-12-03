/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/03 19:09:17 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

Request::~Request() = default;
Request::Request() = default;

void	Request::set_method(const std::string& method) { _method = method; }
void	Request::set_target(const std::string& target) { _target = target; }
void	Request::set_host(const std::string& host) { _host = host; }
void	Request::set_port(const std::string& port) { _port = port; }

void	Request::add_header(const std::pair<std::string, std::string>& header) {
	_headers[header.first] = header.second;
}
const std::string&	Request::get_header(const std::string& header) const {
	return _headers.at(header);
}

const std::string&	dRequest::get_method() const { return (_method); }
const std::string&	Request::get_target() const { return (_target); }
const std::string&	Request::get_host() const { return (_host); }
const std::string&	Request::get_port() const { return (_port); }
const std::string&	Request::get_body() const { return (_body); }
