/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 02:03:04 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 13:01:39 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "HttpResponse.hpp"

const std::pair<const char*, const char*>	HttpResponse::message_phrases[10] = {
		(const std::pair<char*, const char*>&) ("200", "OK"),
		(const std::pair<char*, const char*>&) ("201", "Created"),
		(const std::pair<char*, const char*>&) ("301", "Moved Permanently"),
		(const std::pair<char*, const char*>&) ("302", "Found"),
		(const std::pair<char*, const char*>&) ("400", "Bad Request"),
		(const std::pair<char*, const char*>&) ("403", "Forbidden"),
		(const std::pair<char*, const char*>&) ("404", "Not Found"),
		(const std::pair<char*, const char*>&) ("405", "Method Not Allowed"),
		(const std::pair<char*, const char*>&) ("411", "Length Required"),
		(const std::pair<char*, const char*>&) ("413", "Payload Too Large")
};

void				HttpResponse::setStatusCode(const std::string& status_code_) { status_code = status_code_; }

std::string			HttpResponse::getMessagePhrase(const std::string& code) {
	for (int i = 0; i < 10; ++i) {
		if (HttpResponse::message_phrases[i].first == code)
			return (HttpResponse::message_phrases[i].second);
	}
	return ("Unknown code");
}

void				HttpResponse::generateResponse() {
	struct timeval											tv;
	std::map<std::string, std::string>::const_iterator		it;

	gettimeofday(&tv, 0);
	buffer.add(
			HTTP_VERSION + SP + status_code + SP + getMessagePhrase(status_code) + CRLF
			"Server: " + SERVER_VERSION + CRLF
			"Date: " + ft_getdate(tv) + CRLF);

	if (body.size()) {
		char*	size = ft_itoa(body.size());
		buffer.add("Content-length: " + std::string(size) + CRLF);
		delete []size;
	}
	for (it = headers.begin(); it != headers.end(); ++it)
		buffer.add(it->first + ": " + it->second + CRLF);
	buffer.add(CRLF);
	if (body.size())
		buffer.add(body);
}

int					HttpResponse::sendResponse(int client_socket) {
	int 	bytes;

	bytes = send(client_socket, buffer.c_str(), buffer.size(), 0);
	buffer.erase(0, bytes);
	return (bytes);
}

void	HttpResponse::clear() {
	HttpObject::clear();
	status_code.clear();
	message_phrase.clear();
}

const std::string& HttpResponse::getStatusCode() const { return (status_code); }
