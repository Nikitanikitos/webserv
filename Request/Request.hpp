/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 18:03:37 by imicah            #+#    #+#             */
/*   Updated: 2020/12/18 17:08:19 by imicah           ###   ########.fr       */
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
#include <iostream>

class	Request {
private:
	typedef		std::map<std::string, std::string>		_headers_t;

	std::string				_buffer;
	std::string				_method;
	std::string				_target;
	std::string				_body;
	_headers_t				_headers;

public:
	const std::string& GetBuffer() const;

public:
	Request() { }
	virtual ~Request() { }

	void							AddHeader(const std::string& key, const std::string& value);
	void							SetTarget(const std::string& target);
	void							SetMethod(const std::string& method);

	const std::string&				GetMethod() const;
	const std::string&				GetTarget() const;
	const std::string&				GetBody() const;
	const std::string&				GetHeader(const std::string& header) const;

	void							AddToBuffer(char* data);
	void							Clear();
};

#endif //WEBSERV_REQUEST_HPP
