/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpObject.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 11:43:02 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:06:59 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpObject.hpp"

void				HttpObject::SetBody(const bytes& body_) { body = body_; }

void				HttpObject::AddToBuffer(char* data, size_t n) { buffer.add(data, n); }
void				HttpObject::AddHeader(const std::string& key, const std::string& value)
	{ headers.insert(std::make_pair(key, value)); }

bool				HttpObject::FindHeader(const std::string& header) const { return (headers.count(header)); }
const bytes&		HttpObject::GetBody() const { return (body); }
const bytes&		HttpObject::GetBuffer() const { return (buffer); }
const std::string&	HttpObject::GetHeader(const std::string& header) const { return (headers.at(header)); }

void				HttpObject::Clear() {
	body.clear();
	buffer.clear();
	headers.clear();
}
