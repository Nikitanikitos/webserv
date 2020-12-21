/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpObject.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 11:37:57 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:49:53 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HTTPOBJECT_HPP
# define WEBSERV_HTTPOBJECT_HPP

# include <map>
# include <string>
# include "bytes.hpp"

class HttpObject {
protected:
	typedef		std::map<std::string, std::string>		_headers_t;

	bytes			buffer;
	bytes			body;
	_headers_t		headers;

public:
	HttpObject() { }
	virtual ~HttpObject() { }

	void							addToBuffer(char* data, size_t n);
	void							addHeader(const std::string& key, const std::string& value);
	void							SetBody(const bytes& body);

	const bytes&					getBody() const;
	const bytes&					getBuffer() const;
	bool							findHeader(const std::string& header) const;
	const std::string&				getHeader(const std::string& header) const;

	virtual void					clear();
};

#endif //WEBSERV_HTTPOBJECT_HPP
