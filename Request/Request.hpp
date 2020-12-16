/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 18:03:37 by imicah            #+#    #+#             */
/*   Updated: 2020/12/01 17:02:33 by imicah           ###   ########.fr       */
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

	void							AddHeader(const std::string& key, const std::string& value);

	void							SetPort(const std::string& port);
	void						 	SetHost(const std::string& host);
	void							SetTarget(const std::string& target);
	void							SetMethod(const std::string& method);

	const std::string&				GetHost() const;
	const std::string&				GetMethod() const;
	const std::string&				GetTarget() const;
	const std::string&				GetPort() const;
	const std::string&				GetBody() const;
	const std::string&				GetHeader(const std::string& header);

	void							Clear();
};

#endif //WEBSERV_REQUEST_HPP
