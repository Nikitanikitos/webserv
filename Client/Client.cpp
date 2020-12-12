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
	if ((_stage_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == nullptr)
		throw std::exception();
	if ((_proccess_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == nullptr)
		throw std::exception();
	pthread_mutex_init(_stage_mutex, NULL);
	pthread_mutex_init(_proccess_mutex, NULL);
}
Client::~Client() {
	pthread_mutex_destroy(_stage_mutex);
	pthread_mutex_destroy(_proccess_mutex);
	free(_stage_mutex);
	free(_proccess_mutex);
}

void			Client::next_stage() { _stage++; }

void			Client::set_request(Request* request) { _request = request; }
void			Client::set_response(Response* response) { _response = response; }

Request*		Client::get_request() const { return (_request); }
Response*		Client::get_response() const { return (_response); }

int				Client::get_stage() const { return (_stage); }
int				Client::get_socket() const { return (_socket); }

void			Client::add_to_buffer(char *data) { _buffer.append(data); }

void			Client::lock_stage_mutex() { pthread_mutex_lock(_stage_mutex); }
void			Client::unlock_stage_mutex() { pthread_mutex_unlock(_stage_mutex); }

const std::string& Client::get_buffer() const {
	return _buffer;
}

void Client::set_processed(bool processed) {
	Client::_in_proccessed = processed;
}

bool Client::in_task_queue() { return (_in_proccessed); }

bool Client::ready_to_action(fd_set* set, int stage) const {
	return (_stage == stage && FD_ISSET(_socket, set));
}

void Client::set_stage(int stage) {
	_stage = stage;
}
