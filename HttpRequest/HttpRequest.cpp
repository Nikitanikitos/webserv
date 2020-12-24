/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 19:17:53 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.hpp>
#include "HttpRequest.hpp"

void					HttpRequest::clear() {
	HttpObject::clear();
	method.clear();
	target.clear();
	stage = 0;
}

bytes					HttpRequest::getRequestData() {
	size_t		i;
	i = (stage != parsing_body) ? buffer.find("\r\n") : buffer.size();
	bytes		result = buffer.substr(i);

	(i != -1) ? buffer.erase(i + 2) : buffer.erase(i);
	return (result);
}

void HttpRequest::addDataToRequest(bytes data) {

	while (data.size()) {
		bytes	request_data = getRequestData();
		switch (stage) {
			case parsing_first_line:
				parsingFirstLine(request_data.c_str());
				break;
			case parsing_headers:
				if (!request_data.size())
					HandlerHttpObject::endOfHeaders(request, response);
				else
					parseHeader(request_data.c_str());
				break;
			case parsing_body:
				addToBody(request_data);
				if (getBody().size() >= ft_atoi(getHeader("content-length").c_str())) {
					trimBody(0);
					setStage(completed);
				}
		}
	}
}

void	HttpRequest::parsingFirstLine(std::string line_request) {
	if (std::count(line_request.begin(), line_request.end(), ' ') != 2)
		throw "400";

	std::string		element;
	for (int i = 0; i < 3; ++i) {
		element = line_request.substr(0, line_request.find(' '));
		if (i == 0) {
			if (isValidMethod(element)) setMethod(element);
			else throw "400";
		}
		else if (i == 1)
			setTarget(element);
		else if (element != "HTTP/1.1" && element != "HTTP/1.0")
			throw "400";
		line_request.erase(0, line_request.find(' ') + 1);
	}
	setStage(parsing_headers);
}

bool	HttpRequest::isValidMethod(const std::string& method) {
	for (size_t i = 0; i < 4; ++i)
		if (methods[i] == method) return (true);
	return (false);
}

void	HttpRequest::parseHeader(const std::string& line) {
	size_t		position;
	std::string key;
	std::string value;

	if (std::count(line.begin(), line.end(), ':') < 1)
		throw "400";
	position = line.find(':');
	key = line.substr(0, position);
	position += (line[position + 1] == ' ') ? 2 : 1;
	value = line.substr(position);
	std::for_each(key.begin(), key.end(), ft_tolower);
	addHeader(key, value);
}

void		HttpRequest::endOfHeaders() {
	if (!findHeader("host"))
		throw "400";
	else if (getMethod() == "PUT" || getMethod() == "POST") {
		if (!findHeader("content-length") && !findHeader("transfer-encoding"))
			throw "411";
//		else if (ft_atoi(request->getHeader("content-length").c_str()) > limit_client_body_size) {
//			response->setStatusCode("413");
//			request->setStage(bad_request);
//		}
		else
			setStage(parsing_body);
	}
	else
		setStage(completed);
}

