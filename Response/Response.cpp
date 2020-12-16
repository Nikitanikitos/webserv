/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 02:03:04 by imicah            #+#    #+#             */
/*   Updated: 2020/12/16 20:58:34 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() : _body() { }
Response::Response(const std::string& status_code, const std::string& message_phrase) :
														_status_code(status_code), _message_phrase(message_phrase) { }

Response::~Response() = default;

void				Response::SetStatusCode(const std::string& status_code) { _status_code = status_code; }
void				Response::SetBody(const std::string& body) { _body = body; }

void				Response::AddHeader(const std::string& key, const std::string& value) {
	_headers.insert(std::make_pair(key, value));
	_headers[key] = value; // TODO каково хера...
}

const std::string&	Response::GetHeader(const std::string& key) { return (_headers[key]); }

void				Response::GenerateResponse() {
	_buffer.append(
		HTTP_VERSION + SP + _status_code + SP + _message_phrase + CRLF
		"Server:" + SP + SERVER_VERSION + CRLF
		"Date:" + SP + ft_getdate() + CRLF);

	_buffer.append("Content-length: " + std::to_string(_body.size()));
	_buffer.append(CRLF);
	for (const auto& header : _headers)
		_buffer.append(header.first + ":" + SP + header.second + CRLF);
	_buffer.append(CRLF);
	if (_body.size()) {
		_buffer.append(_body);
		_buffer.append(CRLF);
		_buffer.append(CRLF);
	}
}

int					Response::SendResponse(int client_socket) {
	int 	bytes;

	bytes = send(client_socket, _buffer.c_str(), _buffer.size(), 0);
	_buffer.erase(0, bytes);
	return (bytes);
}

const std::string& Response::GetBuffer() { return (_buffer); }

void	Response::Clear() {
	_status_code.clear();
	_buffer.clear();
	_headers.clear();
	_body.clear();
	_message_phrase.clear();
}

void Response::SetLocationUri(const std::string& location_uri) {
	_location_uri = location_uri;
}

const std::string& Response::GetStatusCode() const {
	return _status_code;
}

