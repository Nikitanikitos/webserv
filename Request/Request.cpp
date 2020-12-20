/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/19 22:56:38 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

void					Request::SetMethod(const std::string& method) { _method = method; }
void					Request::SetTarget(const std::string& target) { _target = target; }

void					Request::AddToBuffer(char* data) { _buffer.add(data); }
void					Request::AddHeader(const std::string& key, const std::string& value)
	{ _headers.insert(std::make_pair(key, value)); }
bool					Request::FindHeader(const std::string& header) const
	{ return (_headers.count(header)); }
const std::string&		Request::GetHeader(const std::string& header) const { return (_headers.at(header)); }

const std::string&		Request::GetMethod() const { return (_method); }
const std::string&		Request::GetTarget() const { return (_target); }
const bytes&			Request::GetBody() const { return (_body); }
const bytes&			Request::GetBuffer() const { return (_buffer); }

void					Request::Clear() {
	_body.clear();
	_buffer.clear();
	_headers.clear();
	_method.clear();
	_target.clear();
}

void Request::SetBody(const std::string& body) { _body = body; }
