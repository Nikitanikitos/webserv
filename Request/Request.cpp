/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/11/22 15:25:50 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

void Request::set_method(const std::string& method) {
	_method = method;
}

void Request::set_target(const std::string& target) {
	_target = target;
}

void Request::set_host(const std::string& host) {
	_host = host;
}

Request::Request() = default;
