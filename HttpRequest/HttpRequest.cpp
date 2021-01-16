/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/24 17:14:59 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

const std::string		HttpRequest::methods[4] =
		{"GET", "HEAD", "POST", "PUT"};

void		HttpRequest::clear() {
	HttpObject::clear();
	method.clear();
	target.clear();
	stage = 0;
}

bytes		HttpRequest::getRequestData(bytes& data) const {
	size_t		i = data.find("\r\n");
	bytes		result = data.substr(i);

	(i != -1) ? data.erase(i + 2) : data.erase(i);
	return (result);
}

void HttpRequest::addDataToRequest(char* data, size_t size) {
	bytes	request_data;

	addToBuffer(data, size);
	if (data[size - 1] != '\n' && data[size - 2] != '\r')
		return;
	while (!buffer.empty()) {
		switch (getStage()) {
			case parsing_first_line:
				parsingFirstLine(getRequestData(buffer).c_str());
				break;
			case parsing_headers:
				request_data = getRequestData(buffer);
				request_data.empty() ? endOfHeaders() : parseHeader(request_data.c_str());
				break;
			case parsing_body:
				if (findHeader("content-length"))
					parsingBodyByContentLength(buffer);
				else if (findHeader("transfer-encoding"))
					parsingBodyByChunked(buffer);
		}
	}
}

void		HttpRequest::parsingBodyByContentLength(bytes& data) {
	addToBody(data);
	data.erase(data.size());
	if (getBody().size() >= ft_atoi(getHeader("content-length").c_str())) {
		if (getBody().size() > ft_atoi(getHeader("content-length").c_str()))
			trimBody(getBody().size() - ft_atoi(getHeader("content-length").c_str()));
		setStage(completed);
	}
}

void		HttpRequest::parsingFirstLine(std::string line_request) {
	if (std::count(line_request.begin(), line_request.end(), ' ') != 2)
		throw std::string("400");

	std::string		element;
	for (int i = 0; i < 3; ++i) {
		element = line_request.substr(0, line_request.find(' '));
		if (i == 0) {
			if (isValidMethod(element)) setMethod(element);
			else throw std::string("400");
		}
		else if (i == 1)
			setTarget(element);
		else if (element != "HTTP/1.1" && element != "HTTP/1.0")
			throw std::string("400");
		line_request.erase(0, line_request.find(' ') + 1);
	}
	setStage(parsing_headers);
}

bool		HttpRequest::isValidMethod(const std::string& method_) {
	for (size_t i = 0; i < 4; ++i)
		if (methods[i] == method_) return (true);
	return (false);
}

void		HttpRequest::parseHeader(const std::string& line) {
	size_t		position;
	std::string key;
	std::string value;

	if (std::count(line.begin(), line.end(), ':') < 1)
		throw std::string("400");
	position = line.find(':');
	key = line.substr(0, position);
	position += (line[position + 1] == ' ') ? 2 : 1;
	value = line.substr(position);
	std::for_each(key.begin(), key.end(), ft_tolower);
	addHeader(key, value);
}

void		HttpRequest::endOfHeaders() {
	if (!findHeader("host"))
		throw std::string("400");
	else if (getMethod() == "PUT" || getMethod() == "POST") {
		if (!findHeader("content-length") && !findHeader("transfer-encoding"))
			throw std::string("411");
		else if (findHeader("transfer-encoding") && getHeader("transfer-encoding") != "chunked")
			throw std::string("501");
		else
			setStage(parsing_body);
	}
	else
		setStage(completed);
}

void		HttpRequest::parsingBodyByChunked(bytes& data) {
	while (!data.empty()) {
		if (chunk_size == -1)
			chunk_size = ft_atoi_hex(getRequestData(data).c_str());
		if (chunk_size == 0)
			setStage(completed);
		else {
			addToBody(data.substr(chunk_size));
			data.erase(chunk_size + 2);
			chunk_size = -1;
		}
	}
}
