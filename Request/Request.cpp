/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/11/30 19:22:51 by imicah           ###   ########.fr       */
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

const std::string&	Request::get_method() const { return (_method); }
const std::string&	Request::get_target() const { return (_target); }
const std::string&	Request::get_host() const { return (_host); }
const std::string&	Request::get_port() const { return (_port); }
const std::string&	Request::get_body() const { return (_body); }

void	Request::set_virtual_server(const std::vector<VirtualServer>& virtual_server) {
	bool					default_vs_flag;
	const VirtualServer		*default_vs;

	default_vs_flag = false;
	for (const auto & server : virtual_server) {
		for (const auto& port : server.get_ports()) {
			if (_port == port) {
				if (!default_vs_flag) {
					default_vs = &server;
					default_vs_flag = true;
				}
				for (const auto& server_name : server.get_server_names())
					if (get_header(HOST) == server_name) {
						default_vs = &server;
						break ;
					}
			}
		}
	}
	_virtual_server = *default_vs;
}

void Request::set_location() {
	_location = _virtual_server._get_location(*this);
}

const Location&			Request::get_location() const { return (_location); }
const VirtualServer&	Request::get_virtual_server() const { return (_virtual_server); }
