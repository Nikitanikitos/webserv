/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 02:03:04 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 16:27:49 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "HttpResponse.hpp"

const std::string	HttpResponse::message_phrases[count_status_code][2] = {
		{"200", "OK"},
		{"201", "Created"},
		{"301", "Moved Permanently"},
		{"302", "Found"},
		{"400", "Bad Request"},
		{"401", "Unauthorized"},
		{"403", "Forbidden"},
		{"404", "Not Found"},
		{"405", "Method Not Allowed"},
		{"411", "Length Required"},
		{"413", "Payload Too Large"},
		{"501", "Not Implemented"}
};

std::string			HttpResponse::getMessagePhrase(const std::string& code) {
	for (int i = 0; i < count_status_code; ++i) {
		if (HttpResponse::message_phrases[i][0] == code)
			return (HttpResponse::message_phrases[i][1]);
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

	char*	size = ft_itoa(body.size());
	buffer.add("Content-length: " + std::string(size) + CRLF);
	delete []size;
	for (it = headers.begin(); it != headers.end(); ++it)
		buffer.add(it->first + ": " + it->second + CRLF);
	buffer.add(CRLF);
	if (body.size())
		buffer.add(body);
}

int					HttpResponse::sendResponse(int client_socket) {
	int 	bytes;

	bytes = send(client_socket, buffer.c_str(), buffer.size(), 0);
	buffer.erase(bytes);
	return (bytes);
}

void	HttpResponse::clear() {
	HttpObject::clear();
	status_code.clear();
	message_phrase.clear();
}
