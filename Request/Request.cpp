/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 11:54:08 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void					Request::SetMethod(const std::string& method) { _method = method; }
void					Request::SetTarget(const std::string& target) { _target = target; }

const std::string&		Request::GetMethod() const { return (_method); }
const std::string&		Request::GetTarget() const { return (_target); }

void Request::clear() {
	HttpObject::Clear();
	_method.clear();
	_target.clear();
}
