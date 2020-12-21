/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:30:08 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

void					HttpRequest::setMethod(const std::string& method) { method = method; }
void					HttpRequest::setTarget(const std::string& target) { target = target; }

const std::string&		HttpRequest::getMethod() const { return (method); }
const std::string&		HttpRequest::getTarget() const { return (target); }

void HttpRequest::clear() {
	HttpObject::clear();
	method.clear();
	target.clear();
}
