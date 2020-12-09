/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 15:36:04 by imicah            #+#    #+#             */
/*   Updated: 2020/12/06 04:18:37 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int client_socket, int stage) : _client_socket(client_socket), _stage(stage) { }
Client::~Client() = default;

void			Client::next_stage() { _stage++; }

void			Client::set_request(Request* request) { _request = request; }
void			Client::set_response(Response* response) { _response = response; }

Request*		Client::get_request() const { return (_request); }
Response*		Client::get_response() const { return (_response); }

int				Client::get_stage() const { return (_stage); }
int				Client::get_socket() const { return (_client_socket); }

void			Client::add_to_buffer(char *data) { _buffer.append(data); }
