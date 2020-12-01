/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 16:04:10 by imicah            #+#    #+#             */
/*   Updated: 2020/12/01 17:09:16 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_EXCEPTIONS_HPP
# define WEBSERV_EXCEPTIONS_HPP

# include <exception>
# include <string>

class	ARequestException : public std::exception {
protected:
	const std::string	_message_phrase;
	const std::string	_status_code;

	ARequestException(std::string& status_code, std::string& message_phrase)
													: _message_phrase(message_phrase), _status_code(status_code) { };

	~ARequestException() override = 0;

	virtual void						send_response(int) = 0;
};

class	Request301Redirect : public ARequestException {
private:
	std::string		_location;

public:
	explicit Request301Redirect(std::string location)
			: ARequestException((std::string&) "301", (std::string&) "Moved Permanently"), _location(location) { }

	~Request301Redirect() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("301 Moved Permanently"); }
	virtual void				send_response(int);
};

class	Request400Error : public ARequestException {
public:
	Request400Error() : ARequestException((std::string&) "400", (std::string&) "Bad Request") { }
	~Request400Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("400 Bad request"); }
	virtual void				send_response(int);
};

class	Request404Error : public ARequestException {
public:
	Request404Error() : ARequestException((std::string&) "404", (std::string&) "Not found") { }
	~Request404Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("404 Not found"); }
	virtual void				send_response(int);
};

class	Request403Error : public ARequestException {
public:
	Request403Error() : ARequestException((std::string&) "403", (std::string&) "Forbidden") { }
	~Request403Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("403 Forbidden"); }
	virtual void				send_response(int);
};

class	Request405Error : public ARequestException {
public:
	Request405Error() : ARequestException((std::string&) "405", (std::string&) "Method Not Allowed") { }
	~Request405Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("405 Method Not Allowed"); }
	virtual void				send_response(int);
};

class	Request505Error : public ARequestException {
public:
	Request505Error() : ARequestException((std::string&) "505", (std::string&) "HTTP Version Not Supported") { }
	~Request505Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override { return ("505 HTTP Version Not Supported"); }
	void						send_response(int);
};

#endif //WEBSERV_EXCEPTIONS_HPP
