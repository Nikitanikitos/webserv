/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 17:20:09 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

void					HttpRequest::setMethod(const std::string& method_) { method = method_; }
void					HttpRequest::setTarget(const std::string& target_) { target = target_; }

const std::string&		HttpRequest::getMethod() const { return (method); }
const std::string&		HttpRequest::getTarget() const { return (target); }

void HttpRequest::clear() {
	HttpObject::clear();
	method.clear();
	target.clear();
}

bytes	HttpRequest::getRequestLine() {
	size_t		i = buffer.find("\r\n");
	bytes		result = buffer.substr(i);

	(i != -1) ? buffer.erase(i + 2) : buffer.erase(i);
	return (result);
}
