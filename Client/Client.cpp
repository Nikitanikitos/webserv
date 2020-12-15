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

Client::Client(int client_socket, int stage) : _socket(client_socket), _stage(stage), _in_proccessed(false) {
	if ((_proccess_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == nullptr)
		throw std::exception();
	pthread_mutex_init(_proccess_mutex, NULL);
}

Client::~Client() {
	pthread_mutex_destroy(_proccess_mutex);
	free(_proccess_mutex);
}

void			Client::next_stage() { _stage++; }

void			Client::set_request(const Request &request) { _request = request; }
void			Client::set_response(const Response &response) { _response = response; }
void			Client::set_processed(bool processed) { _in_proccessed = processed; }
void			Client::set_stage(int stage) { _stage = stage; }

Request&		Client::get_request() { return (_request); }
Response&		Client::get_response() { return (_response); }

int				Client::get_stage() const { return (_stage); }
int				Client::get_socket() const { return (_socket); }

void			Client::add_to_buffer(char *data) { _buffer.append(data); }

const std::string&	Client::get_buffer() const { return (_buffer); }


bool			Client::in_task_queue() { return (_in_proccessed); }

void			Client::clear_buffer() { _buffer.clear(); }
void			Client::clear_response() { _response.clear(); }
void			Client::clear_request() { _request.clear(); }
