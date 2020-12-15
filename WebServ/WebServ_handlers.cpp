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

void	WebServ::parsing_request(Client *client) {
	Request*	request = new Request();

	std::cout << client->get_buffer() << std::endl;

	client->set_request(request);
	client->next_stage();
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
////			case proxy:
////				_proxy_handler(request, virtual_server, location, client_socket);
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

	response->send_response(client->get_socket());
	if (response->get_buffer().empty()) {
		if (response->get_header("Connection") == "close")
			client->set_stage(close_connection_);
		else
			client->set_stage(read_request_);

		delete response;
		delete client->get_request();

		client->set_request(NULL);
		client->set_response(NULL);
		client->clear_buffer();
	}
}

void	WebServ::close_connection(Client* client) {
	close(client->get_socket());
}
