/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 19:17:53 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

void					HttpRequest::setMethod(const std::string& method_) { method = method_; }
void					HttpRequest::setTarget(const std::string& target_) { target = target_; }
void					HttpRequest::setStage(int stage_) { stage = stage_; }

const std::string&		HttpRequest::getMethod() const { return (method); }
const std::string&		HttpRequest::getTarget() const { return (target); }
int						HttpRequest::getStage() const { return (stage); }

void					HttpRequest::clear() {
	HttpObject::clear();
	method.clear();
	target.clear();
	stage = 0;
}

bytes					HttpRequest::getRequestData() {
	size_t		i;
	i = (stage != parsing_body) ? buffer.find("\r\n") : buffer.size();
	bytes		result = buffer.substr(i);

	(i != -1) ? buffer.erase(i + 2) : buffer.erase(i);
	return (result);
}

void					HttpRequest::trimBody(size_t n) { body.rtrim(n); }
