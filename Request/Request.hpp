/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 18:03:37 by imicah            #+#    #+#             */
/*   Updated: 2020/11/26 12:49:09 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUEST_HPP
# define WEBSERV_REQUEST_HPP

# define ACCEPT_CHARSET		"Accept-Charsets"
# define ACCEPT_LANGUAGE	"Accept-Language"
# define ALLOW				"Allow"
# define AUTHORIZATION		"Authorization"
# define CONTENT_LANGUAGE	"Content-Language"
# define CONTENT_LENGTH		"Content-Length"
# define CONTENT_LOCATION	"Content-Location"
# define CONTENT_TYPE		"Content-Type"
# define DATE				"Date"
# define HOST				"Host"
# define LAST_MODIFIED		"Last-Modified"
# define LOCATION			"Location"
# define REFER				"Referer"
# define RETRY_AFTER		"Retry-After"
# define SERVER				"Server"
# define TRANSFER_ENCODING	"Transfer-Encoding"
# define USER_AGENT			"User-Agent"
# define WWW_AUTHENTICATE	"WWW-Authenticate"

# include <string>
# include <map>

class	Request {
private:
	typedef		std::map<std::string, std::string>		_headers_t;

	std::string				_method;
	std::string				_target;
	std::string				_arguments;
	std::string				_host;
	std::string				_port;
	std::string				_body;
	_headers_t				_headers;

public:
	Request();
	virtual ~Request();

	void				add_header(const std::pair<std::string, std::string>&);

	void				set_port(const std::string&);
	void			 	set_host(const std::string&);
	void				set_target(const std::string&);
	void				set_method(const std::string&);

	[[nodiscard]] const std::string& get_host() const;
	[[nodiscard]] const std::string& get_method() const;
	[[nodiscard]] const std::string& get_target() const;
	[[nodiscard]] const std::string& get_port() const;
	[[nodiscard]] const std::string& get_body() const;

	[[nodiscard]] const	std::string& get_header(const std::string&) const;
};

#endif //WEBSERV_REQUEST_HPP
