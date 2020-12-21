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
								: socket(socket), stage(read_request_), in_proccessed(false), host(ip), port(port) {
	struct timeval	tv;

	gettimeofday(&tv, 0);
	connection_time = tv.tv_sec;

	request = new HttpRequest();
	response = new HttpResponse();
}

Client::~Client() {
	delete response;
	delete request;
	close(socket);
}

void					Client::setProcessed(bool processed) { in_proccessed = processed; }
void					Client::setStage(int stage_) { stage = stage_; }

HttpRequest*			Client::getRequest() const { return (request); }
HttpResponse*			Client::getResponse() const { return (response); }
const std::string&		Client::getHost() const { return (host); }
const std::string&		Client::getPort() const { return port; }
int						Client::getStage() const { return (stage); }
int						Client::getSocket() const { return (socket); }

bool					Client::inTaskQueue() { return (in_proccessed); }

void					Client::nextStage() { stage++; }

void					Client::clearResponse() { response->clear(); }
void					Client::clearRequest() { request->clear(); }

void					Client::generateResponse() { response->generateResponse(); }
void					Client::sendResponse() { response->sendResponse(socket); }

bool Client::connectionTimedOut() {
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return ((tv.tv_sec - connection_time) > TimeOut);
}

void Client::setNewConnectionTime() {
	struct timeval	tv;

	gettimeofday(&tv, 0);
	connection_time = tv.tv_sec;
}
