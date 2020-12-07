/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpObject.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 15:36:04 by imicah            #+#    #+#             */
/*   Updated: 2020/12/06 04:18:37 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpObject.hpp"

HttpObject::HttpObject(int client_socket, int stage) : _client_socket(client_socket), _stage(stage) { }
HttpObject::~HttpObject() = default;

void				HttpObject::next_stage() { _stage++; }

void				HttpObject::set_request(const Request& request) { _request = request; }
void				HttpObject::set_response(const Response& response) { _response = response; }

const Request&		HttpObject::get_request() const { return (_request); }
const Response&		HttpObject::get_response() const { return (_response); }

int					HttpObject::get_stage() const { return (_stage); }
int					HttpObject::get_client_socket() const { return (_client_socket); }
