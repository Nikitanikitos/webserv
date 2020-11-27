/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 16:04:10 by imicah            #+#    #+#             */
/*   Updated: 2020/11/26 17:00:57 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_EXCEPTIONS_HPP
# define WEBSERV_EXCEPTIONS_HPP

# include <exception>
# include <string>

class	IRequestException : public std::exception {
protected:
	const std::string	_message_phrase;
	const std::string	_status_code;

	IRequestException(std::string& status_code, std::string& message_phrase)
													: _message_phrase(message_phrase), _status_code(status_code) { };

	~IRequestException() override = 0;

	virtual void						send_response(int) = 0;
};

class	Request400Error : public IRequestException {
public:
	Request400Error() : IRequestException((std::string&) "400", (std::string&) "Bad Request") { }
	~Request400Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("400 Bad request"); }
	virtual void				send_response(int);
};

class	Request404Error : public IRequestException {
public:
	Request404Error() : IRequestException((std::string&) "404", (std::string&) "Not found") { }
	~Request404Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("404 Not found"); }
	virtual void				send_response(int);
};

class	Request405Error : public IRequestException {
public:
	Request405Error() : IRequestException((std::string&) "405", (std::string&) "Method Not Allowed") { }
	~Request405Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("405 Method Not Allowed"); }
	virtual void				send_response(int);
};

class	Request505Error : public IRequestException {
public:
	Request505Error() : IRequestException((std::string&) "505", (std::string&) "HTTP Version Not Supported") { }
	~Request505Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override { return ("505 HTTP Version Not Supported"); }
	void						send_response(int);
};

#endif //WEBSERV_EXCEPTIONS_HPP
