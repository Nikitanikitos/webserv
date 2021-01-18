/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpObject.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 11:37:57 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 20:18:33 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HTTPOBJECT_HPP
# define WEBSERV_HTTPOBJECT_HPP

# include <map>
# include <string>
# include "bytes.hpp"

class	HttpObject {
protected:
	typedef		std::map<std::string, std::string>		_headers_t;

	bytes			buffer;
	bytes			body;
	_headers_t		headers;

public:
	HttpObject() { }
	virtual ~HttpObject() { }

	inline void						addToBuffer(char* data, size_t n) { buffer.add(data, n); }
	inline void						addToBuffer(const bytes& data) { buffer.add(data); }
	inline void						addHeader(const std::string& key, const std::string& value)
		{ headers.insert(std::make_pair(key, value)); }

	inline void						setBody(const bytes& body_) { body = body_; }
	inline void						addToBody(const bytes& data) { body.add(data); }
	inline void						addToBody(const bytes& data, int size) { body.add(data.c_str(), size); }

	inline const bytes&				getBody() const { return (body); }
	inline const bytes&				getBuffer() const { return (buffer); }
	inline bool						findHeader(const std::string& header) const { return (headers.count(header)); }
	inline const std::string&		getHeader(const std::string& header) const { return (headers.at(header)); }

	virtual void					clear();
};

#endif //WEBSERV_HTTPOBJECT_HPP
