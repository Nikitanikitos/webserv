/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseException.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 16:20:35 by imicah            #+#    #+#             */
/*   Updated: 2020/12/16 15:21:36 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseException.hpp"

ResponseException::ResponseException(const std::string& status_code, const std::string& message_phrase,
									 const std::string& error_page) : Response(status_code, message_phrase), _error_page(error_page) { }

void ResponseException::GenerateResponse() {
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

void	ResponseException::Clear() {
	Response::Clear();
	_error_page.clear();
}
