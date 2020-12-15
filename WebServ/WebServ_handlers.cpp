/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_handlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:06:21 by imicah            #+#    #+#             */
/*   Updated: 2020/12/15 02:31:29 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <iostream>

void	WebServ::read_request(Client *client) {
	char	buff[512];
	int 	bytes;
	static int	w;

	ft_memset(buff, 0, 512);
	bytes = recv(client->get_socket(), buff, 512, 0);
	client->add_to_buffer(buff);
	if (client->get_buffer().rfind("\r\n\r\n") != std::string::npos)
		client->next_stage();
}

std::string WebServ::methods[6] = {
		"GET",
		"HEAD",
		"POST",
		"PUT",
		"DELETE",
		"OPTIONS"
};

std::vector<std::string>	WebServ::_getArgs(const std::string &line) const {
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

std::vector<std::string> WebServ::_trimRequest(std::string const& buff) const {
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

bool	WebServ::_checkCountSpace(std::string const& line, int numSpaces) const { //TODO добавить в хедер
	int countSpace = 0;
	for (int i = 0; i < line.size(); ++i)
		if (line[i] == ' ')
			++countSpace;
	return countSpace == numSpaces;
}

bool	WebServ::_checkMethod(std::string method, int size) const { //TODO добавить в хедер
	for (int i = 0; i < size; ++i) {
		if (WebServ::methods[i] == method)
			return true;
	}
	return false;
}

void 	WebServ::_strToLower(std::string& str) const {
	for (int i = 0; i < str.size(); ++i)
		str[i] = std::tolower(str[i]);
}

void	WebServ::parsing_request(Client *client) {

	try {
		Request		request;

		std::vector<std::string> args = _trimRequest(client->get_buffer());
		if (!_checkCountSpace(args[0], 2))
			throw ResponseException("404", "Bad Request", "404.html");
		else {
			std::vector<std::string> line = _getArgs(args[0]);
			if (line.size() != 3 || _checkMethod(args[0], 6) || line[2] != HTTP_VERSION)
				throw ResponseException("404", "Bad Request", "404.html");
			request.set_method(line[0]);
			request.set_target(line[1]);
			for (size_t i = 1; i < args.size(); ++i) {
				line = _getArgs(args[i]);
				if (line.size() == 1 || line.size() > 2 || line[0].back() != ':')
					throw ResponseException("404", "Bad Request", "404.html");
				std::string key = line[0].substr(0, line[0].size() - 1);
				_strToLower(key);
				request.add_header(std::make_pair(key, line[1]));
			}
		}
		client->set_request(request);
		client->next_stage();
	}
	catch (ResponseException& response) {
		response.generate_response();
		client->set_response(response);
		client->set_stage(send_response_);
	}
}

void	WebServ::generate_response(Client *client) {
	Response*	response = new Response();

	try {
		const VirtualServer&	virtual_server = _get_virtual_server(client->get_request());
		const Location&			location = virtual_server.get_location(client->get_request());

		chdir(location.get_root().c_str());
		switch (location.get_location_type()) {
			case default_location:
				_default_handler(client, virtual_server, location);
//			case cgi_location:
//				_cgi_handler(request, virtual_server, location, client_socket);
		}
	}
	catch (Request301Redirect& redirect_301) {
		client->set_response(redirect_301);
	}
	catch (ResponseException& request_error) {
		client->set_response(request_error);
	}
	client->next_stage();
}

void	WebServ::send_response(Client* client) {
	Response&	response = client->get_response();

	response.send_response(client->get_socket());
	if (response.get_buffer().empty()) {
		if (response.get_header("Connection") == "close")
			client->set_stage(close_connection_);
		else
			client->set_stage(read_request_);

		client->clear_response();
		client->clear_request();
		client->clear_buffer();
	}
}

void	WebServ::close_connection(Client* client) {
	close(client->get_socket());
}
