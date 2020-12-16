/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_handlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:06:21 by imicah            #+#    #+#             */
/*   Updated: 2020/12/16 12:13:57 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <iostream>

void	WebServ::ReadRequest(Client *client) {
	char	buff[512];
	int 	bytes;
	static int	w;

	ft_memset(buff, 0, 512);
	bytes = recv(client->GetSocket(), buff, 512, 0);
	client->AddToBuffer(buff);
	if (client->GetBuffer().rfind("\r\n\r\n") != std::string::npos)
		client->NextStage();
}

void	WebServ::GenerateResponse(Client *client) {
	try {
		const VirtualServer&	virtual_server = _GetVirtualServer(client);
		const Location&			location = virtual_server.GetLocation(client->GetRequest());

		chdir(location.GetRoot().c_str());
		_DefaultHandler(client, virtual_server, location);
	}
	catch (Request301Redirect& redirect_301) {
		client->SetResponse(redirect_301);
	}
	catch (ResponseException& request_error) {
		client->SetResponse(request_error);
	}
	client->GetResponse().GenerateResponse();
	client->NextStage();
}

void	WebServ::SendResponse(Client* client) {
	Response&	response = client->GetResponse();

	response.SendResponse(client->GetSocket());
	if (response.GetBuffer().empty()) {
		if (response.GetHeader("Connection") == "close")
			client->SetStage(close_connection_);
		else
			client->SetStage(read_request_);

		client->ClearResponse();
		client->ClearRequest();
		client->ClearBuffer();
	}
}

void	WebServ::CloseConnection(Client* client) {
	close(client->GetSocket());
}
