/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpObject.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 11:37:57 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:06:59 by imicah           ###   ########.fr       */
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

	void							AddToBuffer(char* data, size_t n);
	void							AddHeader(const std::string& key, const std::string& value);
	void							SetBody(const bytes& body);

	const bytes&					GetBody() const;
	const bytes&					GetBuffer() const;
	bool							FindHeader(const std::string& header) const;
	const std::string&				GetHeader(const std::string& header) const;

	virtual void					Clear();
};

#endif //WEBSERV_HTTPOBJECT_HPP
