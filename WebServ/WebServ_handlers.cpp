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

	ft_memset(buff, 0, 512);
	bytes = recv(client->get_socket(), buff, 512, 0);
	client->add_to_buffer(buff);
	if (client->get_buffer().rfind("\r\n\r\n") != std::string::npos)
		client->next_stage();
}

//	GET,
//	HEAD,
//	POST,
//	PUT,
//	DELETE,
//	OPTIONS,

std::string WebServ::methods[6] = {
		"GET",
		"HEAD",
		"POST",
		"PUT",
		"DELETE",
		"OPTIONS"
};

//std::string WebServ::fields[18] = {
//		"accept-charsets:",
//		"accept-language:",
//		"allow:",
//		"authorization:",
//		"content-language:",
//		"content-length:",
//		"content-location:",
//		"content-type:",
//		"date:",
//		"host:",
//		"last-modified:",
//		"location:",
//		"referer:",
//		"retry-after:",
//		"server:",
//		"transfer-encoding:",
//		"user-agent",
//		"www-authenticate:"
//};

std::vector<std::string> _getArgs(std::string const& line) { //TODO добавить в хедер
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

std::vector<std::string> _trimRequest(std::string const& buff) { //TODO добавить в хедер
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

bool _checkCountSpace(std::string const& line, int numSpaces) { //TODO добавить в хедер
	int countSpace = 0;
	for (int i = 0; i < line.size(); ++i)
		if (line[i] == ' ')
			++countSpace;
	return countSpace == numSpaces;
}

bool _checkMethod(std::string method, int size) { //TODO добавить в хедер
	for (int i = 0; i < size; ++i) {
		if (WebServ::methods[i] == method)
			return true;
	}
	return false;
}

void 	strToLower(std::string& str) {
	for (int i = 0; i < str.size(); ++i)
		str[i] = std::tolower(str[i]);
}

void	WebServ::parsing_request(Client *client) {

	try {
		Request*	request = new Request();

		std::cout << client->get_buffer() << std::endl;
		std::vector<std::string> args = _trimRequest(client->get_buffer());
		if (!_checkCountSpace(args[0], 2))
			throw RequestException("404", "Bad Request", "404.html"); //TODO доделать
		else {
			std::vector<std::string> line = _getArgs(args[0]);
			if (line.size() != 3 || _checkMethod(args[0], 6) || line[2] != HTTP_VERSION)
				throw RequestException("404", "Bad Request", "404.html"); //TODO доделать
			request->set_method(line[0]);
			request->set_target(line[1]);
			for (size_t i = 1; i < args.size(); ++i) {
				line = _getArgs(args[i]);
				if (line.size() == 1 || line.size() > 2 || line[0].back() != ':')
					throw RequestException("404", "Bad Request", "404.html"); //TODO доделать
				std::string key = line[0].substr(0, line.size() - 1);
				strToLower(key);
				request->add_header(std::make_pair(key, line[1]));
			}
		}
		client->set_request(request);
		client->next_stage();
	}
	catch (RequestException& response) {
		std::cout << "GOVNO!\n";
		client->set_response(new RequestException(response));
		client->get_response()->generate_response();
		client->set_stage(send_response_);
	}
}

void	WebServ::generate_response(Client *client) {
	Response*	response = new Response();

//	std::cout << "generate response " << client->get_socket() << " client" << std::endl;

	client->next_stage();
	client->set_response(response);
//	try {
//		const VirtualServer&	virtual_server = _get_virtual_server(client->get_request());
//		const Location&			location = virtual_server.get_location(client->get_request());
//
//		chdir(location.get_root().c_str());
//		switch (location.get_location_type()) {
//			case _default:
//				_default_handler(client, virtual_server, location);
////			case cgi:
////				_cgi_handler(request, virtual_server, location, client_socket);
//		}
//	}
//	catch (Request301Redirect* redirect_301) {
//		client->set_response(redirect_301);
//	}
//	catch (RequestException* request_error) {
//		client->set_response(request_error);
//	}
}

void	WebServ::send_response(Client* client) {
	Response*	response = client->get_response();

	std::cout << "Ku!" << std::endl;
	response->send_response(client->get_socket());
	if (response->get_buffer().empty()) {
		if (response->get_header("Connection") == "close")
			client->set_stage(close_connection_);
		else
			client->set_stage(read_request_);

		delete response;
//		if (client->get_request())
//			delete client->get_request();

		client->set_request(NULL);
		client->set_response(NULL);
		client->clear_buffer();
	}
}

void	WebServ::close_connection(Client* client) {
	close(client->get_socket());
}
