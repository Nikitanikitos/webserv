/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:30:05 by nikita            #+#    #+#             */
/*   Updated: 2020/11/21 20:35:52 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route() {
	_accepted_methods.assign(5, non_accepted);
}

void Route::set_root(const std::string& root) {
	_root = root;
}

void Route::set_autoindex(bool autoindex) {
	_autoindex = autoindex;
}

void Route::set_cgi_path(const std::string& cgi_path) {
	_cgi_path = cgi_path;
}

void Route::add_accepted_method(uint8_t method_index) {
	_accepted_methods[method_index] = accepted;
}

Route::~Route() {

}
