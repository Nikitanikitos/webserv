/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 02:03:04 by imicah            #+#    #+#             */
/*   Updated: 2020/12/19 15:47:10 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Response.hpp"

const std::map<std::string, std::string>	Response::_message_phrases = {
		{"200", "OK"},
		{"301", "Moved Permanently"},
		{"302", "Found"},
		{"400", "Bad Request"},
		{"403", "Forbidden"},
		{"404", "Not Found"},
		{"405", "Method Not Allowed"},
};

Response::Response() { }
Response::Response(const std::string& status_code, const std::string& message_phrase) :
														_status_code(status_code), _message_phrase(message_phrase) { }

void				Response::SetStatusCode(const std::string& status_code) { _status_code = status_code; }
void				Response::SetBody(const bytes& body) { _body.add(body); }

void				Response::AddHeader(const std::string& key, const std::string& value)
	{ _headers.insert(std::make_pair(key, value)); }

const std::string&	Response::GetHeader(const std::string& key) const { return (_headers.at(key)); }

void				Response::GenerateResponse() {
	struct timeval	tv;

	gettimeofday(&tv, 0);

	_buffer.add(
			HTTP_VERSION + SP + _status_code + SP + _message_phrases.at(_status_code) + CRLF
																						"Server: " + SERVER_VERSION +
			CRLF
			"Date: " + ft_getdate(tv) + CRLF);

	if (_body.size())
		_buffer.add("Content-length: " + std::to_string(_body.size()) + CRLF);

	for (const auto& header : _headers)
		_buffer.add(header.first + ": " + header.second + CRLF);
	_buffer.add(CRLF);

	if (_body.size()) {
		_buffer.add(_body);
		_buffer.add(CRLF);
		_buffer.add(CRLF);
	}
}

int					Response::SendResponse(int client_socket) {
	int 	bytes;

	bytes = send(client_socket, _buffer.c_str(), _buffer.size(), 0);
	_buffer.erase(0, bytes);
	return (bytes);
}

const bytes& Response::GetBuffer() const { return (_buffer); }

void	Response::Clear() {
	_status_code.clear();
	_buffer.clear();
	_headers.clear();
	_body.clear();
	_message_phrase.clear();
}

const std::string& Response::GetStatusCode() const { return (_status_code); }
