/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 18:03:37 by imicah            #+#    #+#             */
/*   Updated: 2020/12/17 13:18:16 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUEST_HPP
# define WEBSERV_REQUEST_HPP

# define ACCEPT_CHARSET		"accept-charsets"
# define ACCEPT_LANGUAGE	"accept-language"
# define ALLOW				"allow"
# define AUTHORIZATION		"authorization"
# define CONTENT_LANGUAGE	"content-language"
# define CONTENT_LENGTH		"content-length"
# define CONTENT_LOCATION	"content-location"
# define CONTENT_TYPE		"content-type"
# define DATE				"date"
# define HOST				"host"
# define CONNECTION			"connection"
# define LAST_MODIFIED		"last-modified"
# define LOCATION			"location"
# define REFER				"referer"
# define RETRY_AFTER		"retry-after"
# define SERVER				"server"
# define TRANSFER_ENCODING	"transfer-encoding"
# define USER_AGENT			"user-agent"
# define WWW_AUTHENTICATE	"www-authenticate"

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
	Request() { }
	virtual ~Request() { }

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
	const std::string&				GetHeader(const std::string& header) const;

	void							Clear();
};

#endif //WEBSERV_REQUEST_HPP
