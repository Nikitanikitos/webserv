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

#include "HttpResponse.hpp"

const std::map<std::string, std::string>	HttpResponse::message_phrases = {
		{"200", "OK"},
		{"201", "Created"},
		{"301", "Moved Permanently"},
		{"302", "Found"},
		{"400", "Bad Request"},
		{"403", "Forbidden"},
		{"404", "Not Found"},
		{"405", "Method Not Allowed"},
		{"411", "Length Required"},
		{"413", "Payload Too Large"}
};

void				HttpResponse::setStatusCode(const std::string& status_code_) { status_code = status_code_; }

void				HttpResponse::generateResponse() {
	struct timeval											tv;
	std::map<std::string, std::string>::const_iterator		it;

	gettimeofday(&tv, 0);
	buffer.add(
			HTTP_VERSION + SP + status_code + SP + message_phrases.at(status_code) + CRLF
			"Server: " + SERVER_VERSION + CRLF
			"Date: " + ft_getdate(tv) + CRLF);

	if (body.size())
		buffer.add("Content-length: " + std::to_string(body.size()) + CRLF);
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
