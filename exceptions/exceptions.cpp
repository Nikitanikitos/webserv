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

#include "exceptions.hpp"

RequestException::RequestException(const std::string& status_code, const std::string& message_phrase,
		const std::string& error_page) : _message_phrase(message_phrase), _status_code(status_code),
										 _error_page(error_page) { }

void RequestException::generate_response(int client_socket) const {
	std::string			body_response = ft_getfile(_error_page.c_str());
	std::string			response;

	response =
			HTTP_VERSION + SP + _status_code + SP + _message_phrase + CRLF
			"Server:" + SP + SERVER_VERSION + CRLF
			"Date:" + SP + ft_getdate() + CRLF
			"Content-type: text/html" + CRLF
			"Content-length:" + SP + std::to_string(body_response.length()) + CRLF
			"Connection:" + SP + "close" + CRLF CRLF +
			body_response;

	send(client_socket, response.c_str(), response.length(), 0);
}

Request301Redirect::Request301Redirect(const std::string& location)  : _location(location) { }

void Request301Redirect::generate_response(int client_socket) const {
	std::string			response;

	response =
			HTTP_VERSION + SP + _status_code + SP + _message_phrase + CRLF
			"Server:" + SP + SERVER_VERSION + CRLF
			"Date:" + SP + ft_getdate() + CRLF
			"Location:" + SP + _location + CRLF
			"Connection:" + SP + "close" + CRLF CRLF;

	send(client_socket, response.c_str(), response.length(), 0);

}

