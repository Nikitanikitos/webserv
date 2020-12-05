/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 16:04:10 by imicah            #+#    #+#             */
/*   Updated: 2020/12/02 17:07:34 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_EXCEPTIONS_HPP
# define WEBSERV_EXCEPTIONS_HPP

# include <exception>
# include <string>
# include <fstream>
#include <Response.hpp>
# include "libft.hpp"

class	RequestException : public std::exception, public Response {
private:
	const std::string	_message_phrase;
	const std::string	_status_code;
	const std::string	_error_page;

public:
	RequestException(const std::string& status_code, const std::string& message_phrase, const std::string &error_page)
			: _message_phrase(message_phrase), _status_code(status_code),
			  _error_page(error_page) { };

	~RequestException() override = default;;

	void	send_response(int client_socket) const override {
		std::ifstream		file(_error_page);
		std::string			body_response;
		std::string			response;

		getline(file, body_response, '\0');
		response =
			HTTP_VERSION + SP + _status_code + SP + _message_phrase + CRLF
			"Server:" + SP + SERVER_VERSION + CRLF
			"Date:" + SP + ft_getdate() + CRLF
			"Content-type: text/html" + CRLF
			"Content-length:" + SP + std::to_string(body_response.length()) + CRLF
			"Connection: keep-alive" + CRLF CRLF +
			body_response;

		send(client_socket, response.c_str(), response.length(), 0);
	}
};

class	Request301Redirect : public std::exception, public Response  {
private:
	const std::string	_message_phrase;
	const std::string	_status_code;
	std::string			_location;

public:
	explicit Request301Redirect(std::string location) : _location(location) { }

	~Request301Redirect() = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT { return ("301 Moved Permanently"); }
	void						send_response(int) { }
};

#endif //WEBSERV_EXCEPTIONS_HPP
