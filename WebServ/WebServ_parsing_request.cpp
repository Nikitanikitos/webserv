/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_parsing_request.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 20:06:14 by imicah            #+#    #+#             */
/*   Updated: 2020/12/16 11:56:33 by imicah           ###   ########.fr       */
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

std::vector<std::string>	WebServ::_GetArgs(const std::string &line) const {
	std::vector<std::string>	result;
	size_t						pos_find;

	std::string input(line);
	while (!input.empty()){
		pos_find = input.find(' ');
		input.erase(pos_find, input.find_first_not_of(' '));
		pos_find = input.find(' ');
		result.push_back(input.substr(0, pos_find));
		input.erase(0, pos_find);
	}
	return (result);
}

std::vector<std::string> WebServ::_TrimRequest(std::string const& buff) const {
	std::vector<std::string> result;
	std::string::size_type start = 0;
	std::string::size_type pos = 0;
	while (true) {
		pos = buff.find("\r\n", start);
		result.push_back(buff.substr(start, pos - start));
		if (!buff.compare(pos, 4, "\r\n\r\n"))
			break ;
		start = pos + 2;
	}
	return result;
}

bool	WebServ::_CheckCountSpace(std::string const& line, int numSpaces) const { //TODO добавить в хедер
	int countSpace = 0;
	for (int i = 0; i < line.size(); ++i)
		if (line[i] == ' ')
			++countSpace;
	return countSpace == numSpaces;
}

bool	WebServ::_CheckMethod(std::string method, int size) const { //TODO добавить в хедер
	for (int i = 0; i < size; ++i) {
		if (WebServ::methods[i] == method)
			return true;
	}
	return false;
}

void 	WebServ::_StrToLower(std::string& str) const {
	for (int i = 0; i < str.size(); ++i)
		str[i] = std::tolower(str[i]);
}

void	WebServ::ParsingRequest(Client *client) {

	try {
		Request		request;
		bool 		takeHost = false;
		std::vector<std::string> args = _TrimRequest(client->GetBuffer());
		if (!_CheckCountSpace(args[0], 2))
			throw ResponseException("400", "Bad Request", "400.html");
		else {
			std::vector<std::string> line = _GetArgs(args[0]);
			if (line.size() != 3 || _CheckMethod(args[0], 6) || line[2] != HTTP_VERSION)
				throw ResponseException("400", "Bad Request", "400.html");
			request.SetMethod(line[0]);
			request.SetTarget(line[1]);
			for (size_t i = 1; i < args.size(); ++i) {
				line = _GetArgs(args[i]);
				_StrToLower(line[0]);
				if (line.size() == 1 || line.size() > 2 || line[0].back() != ':')
					throw ResponseException("400", "Bad Request", "400.html");
				std::string key = line[0].substr(0, line[0].size() - 1);
				if (key == "host")
					takeHost = true;
				request.AddHeader(key, line[1]);
			}
			if (!takeHost)
				throw ResponseException("400", "Bad Request", "400.html");
		}
		client->SetRequest(request);
		client->NextStage();
	}
	catch (ResponseException& response) {
		response.GenerateResponse();
		client->SetResponse(response);
		client->SetStage(send_response_);
	}
}
