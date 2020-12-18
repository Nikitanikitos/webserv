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

Client::Client(int socket, const std::string& ip, const std::string& port)
								: _socket(socket), _stage(read_request_), _in_proccessed(false), _ip(ip), _port(port) {
	struct timeval	tv;

	gettimeofday(&tv, 0);
	_connection_time = tv.tv_sec;

	_request = new Request();
	_response = new Response();
}

Client::~Client() {
	delete _response;
	delete _request;
	close(_socket);
}

void				Client::SetProcessed(bool processed) { _in_proccessed = processed; }
void				Client::SetStage(int stage) { _stage = stage; }

Request*			Client::GetRequest() const { return (_request); }
Response*			Client::GetResponse() const { return (_response); }
const std::string&	Client::GetIp() const { return (_ip); }
const std::string&	Client::GetPort() const { return _port; }
int					Client::GetStage() const { return (_stage); }
int					Client::GetSocket() const { return (_socket); }

bool				Client::InTaskQueue() { return (_in_proccessed); }

void				Client::NextStage() { _stage++; }

void				Client::ClearResponse() { _response->Clear(); }
void				Client::ClearRequest() { _request->Clear(); }

void				Client::GenerateResponse() { _response->GenerateResponse(); }
void				Client::SendResponse() { _response->SendResponse(_socket); }

bool Client::ConnectionTimedOut() {
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return ((tv.tv_sec - _connection_time) > TimeOut);
}

void Client::SetNewConnectionTime() {
	struct timeval	tv;

	gettimeofday(&tv, 0);
	_connection_time = tv.tv_sec;
}

