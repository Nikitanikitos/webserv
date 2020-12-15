/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 16:20:35 by imicah            #+#    #+#             */
/*   Updated: 2020/12/06 02:26:19 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseException.hpp"

ResponseException::ResponseException(const std::string& status_code, const std::string& message_phrase,
									 const std::string& error_page) : _error_page(error_page) { }

void	ResponseException::generate_response(int client_socket) {
	std::string			body_response = ft_getfile(_error_page.c_str());
	std::string			response;

	_buffer.append(
			HTTP_VERSION + SP + _status_code + SP + _message_phrase + CRLF
			"Server:" + SP + SERVER_VERSION + CRLF
		  	"Date:" + SP + ft_getdate() + CRLF
		  	"Content-type: text/html" + CRLF
		  	"Content-length:" + SP + std::to_string(body_response.length()) + CRLF
		  	"Connection:" + SP + "close" + CRLF CRLF +
			body_response);
}

void	ResponseException::clear() {
	Response::clear();
	_error_page.clear();
}

Request301Redirect::Request301Redirect(const std::string& location)  : _location(location) { }

void	Request301Redirect::generate_response(int client_socket) const {
	std::string			response;

	response =
			HTTP_VERSION + SP + _status_code + SP + _message_phrase + CRLF
			"Server:" + SP + SERVER_VERSION + CRLF
			"Date:" + SP + ft_getdate() + CRLF
			"Location:" + SP + _location + CRLF
			"Connection:" + SP + "close" + CRLF CRLF;
}
