/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 02:03:04 by imicah            #+#    #+#             */
/*   Updated: 2020/12/04 00:47:44 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() : _body() { }
Response::Response(std::string& status_code) : _status_code(status_code), _body() { }
Response::~Response() = default;

void				Response::set_status_code(const std::string& status_code) { _status_code = status_code; }
void				Response::set_body(const std::string& body) { _body = body; }

void				Response::add_header(const std::string& key, const std::string& value) {
	_headers.insert(std::make_pair(key, value));
}

const std::string&	Response::get_header(const std::string& key) const { return _headers.at(key); }

void				Response::generate_response() {
	_response.append(
		HTTP_VERSION + SP + _status_code + SP + _message_phrase + CRLF
		"Server:" + SP + SERVER_VERSION + CRLF
		"Date:" + SP + ft_getdate() + CRLF);

	for (const auto& header : _headers)
		_response.append(header.first + ":" + SP + header.second + CRLF);
	_response.append(CRLF);
	_response.append(_body);
}

int					Response::send_response(int client_socket) {
	int 	bytes;

	bytes = send(client_socket, _response.c_str(), 512, 0);
	_response.erase(0, 512);
	return (bytes);
}
