/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_handlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:06:21 by imicah            #+#    #+#             */
/*   Updated: 2020/12/12 08:09:30 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <iostream>

void	WebServ::read_request(Client *client) {
	char	buff[512];
	int 	bytes;

	bytes = recv(client->get_socket(), buff, 512, 0);
	client->add_to_buffer(buff);
	if (bytes < 512)
		client->next_stage();
}

void	WebServ::parsing_request(Client *client) {
	std::cout << "generate request " << client->get_socket() << " client" << std::endl;
	client->next_stage();
}

void	WebServ::generate_response(Client *client) {
	std::cout << "generate response " << client->get_socket() << " client" << std::endl;
	client->next_stage();
	client->set_response(new Response());
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

void	WebServ::send_response(Client* http_object) {

}

void	WebServ::close_connection(Client* client) {
	close(client->get_socket());
}
