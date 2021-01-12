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
								: socket(socket), stage(parsing_request), in_proccessed(false), host(ip), port(port) {
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

bool					Client::connectionTimedOut() {
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return ((tv.tv_sec - connection_time) > TimeOut);
}

void					Client::setNewConnectionTime() {
	struct timeval	tv;

	gettimeofday(&tv, 0);
	connection_time = tv.tv_sec;
}
