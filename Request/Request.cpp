/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/18 01:53:40 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

void	Request::SetMethod(const std::string& method) { _method = method; }
void	Request::SetTarget(const std::string& target) { _target = target; }

void					Request::AddToBuffer(char *data) { _buffer.append(data); }
void					Request::AddHeader(const std::string key, const std::string value)
	{ _headers.insert(std::make_pair(key, value)); }

const std::string&		Request::GetHeader(const std::string& header) const { return (_headers.at(header)); }
const std::string&		Request::GetMethod() const { return (_method); }
const std::string&		Request::GetTarget() const { return (_target); }
const std::string&		Request::GetBody() const { return (_body); }
const std::string&		Request::GetBuffer() const { return (_buffer); }


void					Request::Clear() {
	_body.clear();
	_buffer.clear();
//	_headers.clear();
	_method.clear();
	_target.clear();
}
