/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/17 10:49:43 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

Request::~Request() = default;
Request::Request() = default;

void	Request::SetMethod(const std::string& method) { _method = method; }
void	Request::SetTarget(const std::string& target) { _target = target; }
void	Request::SetHost(const std::string& host) { _host = host; }
void	Request::SetPort(const std::string& port) { _port = port; }

void Request::AddHeader(const std::string& key, const std::string& value)
	{ _headers.insert(std::make_pair(key, value)); }

const std::string&	Request::GetHeader(const std::string& header) const { return (_headers.at(header)); }
const std::string&	Request::GetMethod() const { return (_method); }
const std::string&	Request::GetTarget() const { return (_target); }
const std::string&	Request::GetHost() const { return (_host); }
const std::string&	Request::GetPort() const { return (_port); }
const std::string&	Request::GetBody() const { return (_body); }

void Request::Clear() {
	_body.clear();
	_headers.clear();
	_arguments.clear();
	_host.clear();
	_port.clear();
	_method.clear();
	_target.clear();
}
