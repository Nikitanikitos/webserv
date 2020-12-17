/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_parsing_request.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 20:06:14 by imicah            #+#    #+#             */
/*   Updated: 2020/12/18 02:02:22 by imicah           ###   ########.fr       */
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

std::vector<std::string> WebServ::_GetArgs(const std::string &line, char separate) const {
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

std::vector<std::string>	WebServ::_GetKeyValue(const std::string &line) const {
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

std::vector<std::string> WebServ::_TrimRequest(std::string const& buff) const {
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
	return (result);
}

bool	WebServ::_CheckCountSpace(std::string const& line, int num_spaces) const {
	int count_space = 0;

	for (int i = 0; i < line.size(); ++i)
		if (line[i] == ' ')
			++count_space;
	return count_space == num_spaces;
}

bool	WebServ::_CheckMethod(std::string method, int size) const {
	for (int i = 0; i < size; ++i) {
		if (WebServ::methods[i] == method)
			return (true);
	}
	return (false);
}

void 	WebServ::_StrToLower(std::string& str) const {
	for (int i = 0; i < str.size(); ++i)
		str[i] = std::tolower(str[i]);
}

void	WebServ::ParsingRequest(Client *client) {
	Request*					request = client->GetRequest();
	Response*					response = client->GetResponse();
	bool						take_host;
	std::vector<std::string>	line;
	std::vector<std::string>	args;

	take_host = false;
	args = _TrimRequest(request->GetBuffer());
	if (_CheckCountSpace(args[0], 2)) {
		line = _GetArgs(args[0], ' ');
		if (line.size() != 3 || _CheckMethod(args[0], 6) || line[2] != HTTP_VERSION)
			throw ResponseException("400", "Bad Request", "400.html");
		request->SetMethod(line[0]);
		request->SetTarget(line[1]);
		for (size_t i = 1; i < args.size(); ++i) {
			line = _GetKeyValue(args[i]);
			_StrToLower(line[0]);
			if (line.empty() || line.size() == 1 || line.size() > 2)
				throw ResponseException("400", "Bad Request", "400.html");
			std::string key = line[0].substr(0, line[0].size());
			if (key == "host")
				take_host = true;
			request->AddHeader(key, line[1]);
		}
		if (!take_host)
			throw ResponseException("400", "Bad Request", "400.html");
		client->NextStage();
	}
	else {
		response->SetStatusCode("404");
		client->SetStage(send_response_);
	}
}
