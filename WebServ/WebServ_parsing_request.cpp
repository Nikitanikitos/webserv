/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_parsing_request.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 20:06:14 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 22:30:41 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <iostream>

std::string WebServ::methods[6] = {
		"GET",
		"HEAD",
		"POST",
		"PUT",
		"DELETE",
		"OPTIONS"
};

int 					WebServ::countSpace(const std::string &line) const {
	size_t 		count_space = 0;

	for (size_t i; i < line.size(); ++i)
		if (line[i] == ' ')
			++count_space;
	return (count_space);
}

std::vector<std::string>	WebServ::getArgs(const std::string &line, char separate) const {
	std::vector<std::string>	result;
	std::string					input(line);
	size_t						pos_find;

	while (!input.empty()){
		pos_find = input.find(separate);
		input.erase(pos_find, input.find_first_not_of(separate));
		pos_find = input.find(separate);
		result.push_back(input.substr(0, pos_find));
		input.erase(0, pos_find);
	}
	return (result);
}

std::vector<std::string>	WebServ::getKeyValue(const std::string &line) const {
	std::vector<std::string>	result;
	std::string					clear_line;
	size_t						pos_find;
	size_t						start;
	size_t						end;

	if((pos_find = line.find(':')) == std::string::npos)
		return (result);
	result.push_back(line.substr(0, pos_find));
	clear_line = line.substr(pos_find + 1);
	start = clear_line.find_first_not_of(' ');
	end = clear_line.find_last_not_of(' ');
	result.push_back(clear_line.substr(start, end));
	return (result);
}

std::vector<std::string> WebServ::trimRequest(std::string const& buff, HttpRequest* request) const {
	std::vector<std::string>	result;
	std::string::size_type		start;
	std::string::size_type 		pos;

	start = 0;
	while (true) {
		pos = buff.find("\r\n", start);
		result.push_back(buff.substr(start, pos - start));
		if (!buff.compare(pos, 4, "\r\n\r\n"))
			break ;
		start = pos + 2;
	}
	pos = buff.find("\r\n\r\n", start) + 4;
	request->SetBody(buff.substr(pos));
	return (result);
}

bool	WebServ::checkCountSpace(std::string const& line, int numSpaces) const {
	int count_space = 0;

	for (int i = 0; i < line.size(); ++i)
		if (line[i] == ' ')
			++count_space;
	return (count_space == numSpaces);
}

bool WebServ::checkMethod(std::string method) const {
	for (int i = 0; i < WebServ::methods->size(); ++i)
		if (WebServ::methods[i] == method)
			return (true);
	return (false);
}

void 	WebServ::strToLower(std::string& str) const {
	for (int i = 0; i < str.size(); ++i)
		str[i] = std::tolower(str[i]);
}

void	WebServ::setBadRequestResponse(Client* client) {
	HttpResponse*	response = client->getResponse();

	response->setStatusCode("400");
	response->SetBody(generateErrorPage(response->getStatusCode()));
	response->generateResponse();
	client->setStage(send_response_);
}

void	WebServ::parsingRequest(Client *client) {
	HttpRequest*				request = client->getRequest();
	bool						take_host;
	std::vector<std::string>	line;
	std::vector<std::string>	args;

	take_host = false;
	args = trimRequest(request->getBuffer().c_str(), request);
	if (!checkCountSpace(args[0], 2)) {
		setBadRequestResponse(client);
		return;
	}
	line = getArgs(args[0], ' ');
	if (line.size() != 3 || checkMethod(args[0])) { // line[2] != "HTTP/1.0"
		setBadRequestResponse(client);
		return;
	}
	request->setMethod(line[0]);
	request->setTarget(line[1]);
	for (size_t i = 1; i < args.size(); ++i) {
		line = getKeyValue(args[i]);
		strToLower(line[0]);
		if (line.empty() || line.size() == 1 || line.size() > 2) {
			setBadRequestResponse(client);
			return;
		}
		std::string key = line[0].substr(0, line[0].size());
		if (key == "host")
			take_host = true;
		request->addHeader(key, line[1]);
	}
	(!take_host) ? setBadRequestResponse(client) : client->nextStage();
}
