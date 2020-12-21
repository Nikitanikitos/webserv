/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpObject.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 11:43:02 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:12:24 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpObject.hpp"

void				HttpObject::SetBody(const bytes& body_) { body = body_; }

void				HttpObject::addToBuffer(char* data, size_t n) { buffer.add(data, n); }
void				HttpObject::addHeader(const std::string& key, const std::string& value)
	{ headers.insert(std::make_pair(key, value)); }

bool				HttpObject::findHeader(const std::string& header) const { return (headers.count(header)); }
const bytes&		HttpObject::getBody() const { return (body); }
const bytes&		HttpObject::getBuffer() const { return (buffer); }
const std::string&	HttpObject::getHeader(const std::string& header) const { return (headers.at(header)); }

void				HttpObject::clear() {
	body.clear();
	buffer.clear();
	headers.clear();
}
