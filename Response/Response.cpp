/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 02:03:04 by imicah            #+#    #+#             */
/*   Updated: 2020/12/02 17:01:41 by imicah           ###   ########.fr       */
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

void				Response::send_response(int client_socket) {
	std::string			body_response;
	std::string			response;

	response =
		HTTP_VERSION + SP + _status_code + SP + _message_phrase + CRLF
		"Server:" + SP + SERVER_VERSION + CRLF
		"Date:" + SP + ft_getdate() + CRLF
		"Last-Modified:" + SP + get_header("Last-modified") + CRLF
		"Content-length:" + SP + get_header("Content-length") + CRLF
		"Connection: close" + CRLF CRLF +
		_body;

	send(client_socket, response.c_str(), response.length(), 0);
}
