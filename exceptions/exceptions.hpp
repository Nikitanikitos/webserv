/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 16:04:10 by imicah            #+#    #+#             */
/*   Updated: 2020/12/01 20:00:04 by imicah           ###   ########.fr       */
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
	const std::string	_error_page;

	ARequestException(const std::string& status_code, const std::string& message_phrase, const std::string &error_page)
												: _message_phrase(message_phrase), _status_code(status_code),
																							_error_page(error_page) { };

	~ARequestException() override = 0;

	virtual void						send_response(int) = 0;
};

class	Request301Redirect : public std::exception  {
private:
	const std::string	_message_phrase;
	const std::string	_status_code;
	std::string			_location;

public:
	explicit Request301Redirect(std::string location) : _location(location) {

	}

	~Request301Redirect() = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT { return ("301 Moved Permanently"); }
	void						send_response(int);
};

class	Request400Error : public ARequestException {
public:
	explicit Request400Error(const std::string& error_pages) : ARequestException(
							(std::string&) "400", (std::string&) "Bad Request", error_pages) { }

	~Request400Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("400 Bad request"); }
	virtual void				send_response(int);
};

class	Request404Error : public ARequestException {
public:
	Request404Error(const std::string& error_pages) : ARequestException(
								(std::string&) "404", (std::string&) "Not found", error_pages) { }
	~Request404Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("404 Not found"); }
	virtual void				send_response(int);
};

class	Request403Error : public ARequestException {
public:
	Request403Error(const std::string& error_pages) : ARequestException(
							(std::string&) "403", (std::string&) "Forbidden", error_pages) { }
	~Request403Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("403 Forbidden"); }
	virtual void				send_response(int);
};

class	Request405Error : public ARequestException {
public:
	Request405Error(const std::string& error_pages) : ARequestException(
					(std::string&) "405", (std::string&) "Method Not Allowed", error_pages) { }
	~Request405Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override  { return ("405 Method Not Allowed"); }
	virtual void				send_response(int);
};

class	Request505Error : public ARequestException {
public:
	Request505Error(const std::string& error_pages) : ARequestException(
			(std::string&) "505", (std::string&) "HTTP Version Not Supported", error_pages) { }
	~Request505Error() override = default;

	[[nodiscard]] const char*	what() const _NOEXCEPT override { return ("505 HTTP Version Not Supported"); }
	void						send_response(int);
};

#endif //WEBSERV_EXCEPTIONS_HPP
