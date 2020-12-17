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

Client::Client(int socket, int stage, const std::string& ip, const std::string& port)
										: _socket(socket), _ip(ip), _port(port), _stage(stage), _in_proccessed(false) {
	if ((_proccess_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == nullptr)
		throw std::exception();
	pthread_mutex_init(_proccess_mutex, 0);
}

Client::~Client() {
	pthread_mutex_destroy(_proccess_mutex);
	free(_proccess_mutex);
}

void			Client::NextStage() { _stage++; }

void			Client::SetRequest(const Request &request) { _request = request; }
void			Client::SetResponse(const Response &response) { _response = response; }
void			Client::SetProcessed(bool processed) { _in_proccessed = processed; }
void			Client::SetStage(int stage) { _stage = stage; }

const Request&		Client::GetRequest() const { return (_request); }
const Response&		Client::GetResponse() const { return (_response); }

int				Client::GetStage() const { return (_stage); }
int				Client::GetSocket() const { return (_socket); }

void			Client::AddToBuffer(char *data) { _buffer.append(data); }

const std::string&	Client::GetBuffer() const { return (_buffer); }


bool			Client::InTaskQueue() { return (_in_proccessed); }

void			Client::ClearBuffer() { _buffer.clear(); }
void			Client::ClearResponse() { _response.Clear(); }
void			Client::ClearRequest() { _request.Clear(); }

const std::string& Client::GetIp() const { return (_ip);
}

const std::string& Client::GetPort() const { return _port; }

void Client::GenerateResponse() {
	_response.GenerateResponse();
}

void Client::SendResponse() {
	_response.SendResponse(_socket);
}
