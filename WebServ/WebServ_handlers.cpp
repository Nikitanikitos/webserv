/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_handlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:06:21 by imicah            #+#    #+#             */
/*   Updated: 2020/12/17 15:37:16 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <iostream>

void	WebServ::ReadRequest(Client *client) {
	char	buff[512];

	ft_memset(buff, 0, 512);
	recv(client->GetSocket(), buff, 512, MSG_TRUNC);
	if (*((int*)buff) == -33557249)
		client->SetStage(close_connection_);
	else {
		client->AddToBuffer(buff);
		if (client->GetBuffer().rfind("\r\n\r\n") != std::string::npos)
			client->NextStage();
	}
}

bool	WebServ::_CheckError(Response& response, Client *client, const VirtualServer& virtual_server, struct stat& buff,
																					  std::string& path_to_target) {
	const Location&			location = virtual_server.GetLocation(client->GetRequest());
	const Request&			request = client->GetRequest();
	int 					fd;

	if (stat(path_to_target.c_str(), &buff) == -1) {
		response.SetStatusCode("404");
		return (true);
	}
	if (S_ISDIR(buff.st_mode) && request.GetTarget().back() != '/') {
		response.SetStatusCode("301");
		response.AddHeader("Location", "http://" + virtual_server.GetIp() + ":" + virtual_server.GetPort() + request.GetTarget() + "/");
		return (true);
	}
	else if (!location.IsAllowMethod(request.GetMethod())) {
		response.SetStatusCode("405");
		return (true);
	}
	if (S_ISDIR(buff.st_mode) && (fd = open((path_to_target + location.GetIndex()).c_str(), O_RDONLY)) != -1) {
		path_to_target.append(location.GetIndex());
		stat(path_to_target.c_str(), &buff);
		close(fd);
	}
	else if (S_ISDIR(buff.st_mode) && !location.GetAutoindex()) {
		response.SetStatusCode("403");
		return (true);
	}
	else if (request.GetMethod() == "POST" && S_ISDIR(buff.st_mode)) {
		response.SetStatusCode("403");
		return (true);
	}
	else if (request.GetMethod() == "POST") {
		response.SetStatusCode("405");
		return (true);
	}
	return (false);
}

std::string WebServ::_GenerateErrorPage(const std::string& code) const {
	return ("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" "
		 	"content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\" "
			"content=\"ie=edge\"><title>"+ code + " " + Response::_message_phrases.at(code) +"</title><style>h1, p {text-align: center;}</style></head><body>"
			"<h1>" + code + " " + Response::_message_phrases.at(code) + "</h1><hr><p>WebServ/0.1</p></body></html>");
}

void WebServ::_SetErrorPage(Response& response, const Location& location, const VirtualServer& virtual_server,
							const Request& request) {
	std::string		path_to_target;

	try {
		path_to_target.append(location.GetPath() + virtual_server.GetErrorPage(response.GetStatusCode()));
		response.AddHeader("Location", "http://" + virtual_server.GetIp() + ":" + virtual_server.GetPort() + path_to_target);
		response.SetStatusCode("302");
	}
	catch (std::exception&) {
		response.SetBody(_GenerateErrorPage(response.GetStatusCode()));
	}
}

void	WebServ::_DefaultHandler(Response& response, Client *client, const VirtualServer& virtual_server,
							  				struct stat& buff, std::string& path_to_target, const Location& location) {
	const Request&		request = client->GetRequest();
	std::string			body;
	struct timeval		tv;

	if (S_ISREG(buff.st_mode) || S_ISLNK(buff.st_mode)) {
		body = ft_getfile(path_to_target.c_str());
		tv.tv_sec = buff.st_mtimespec.tv_sec;
		tv.tv_usec = buff.st_mtimespec.tv_nsec;
		response.AddHeader("Last-modified", ft_getdate(tv));
	}
	else if (S_ISDIR(buff.st_mode) && location.GetAutoindex())
		body = _AutoindexGenerate(request, path_to_target);

	response.SetStatusCode("200");
	if (request.GetMethod() == "GET")
		response.SetBody(body);
}

void	WebServ::GenerateResponse(Client *client) {
	const VirtualServer&	virtual_server = _GetVirtualServer(client);
	const Location&			location = virtual_server.GetLocation(client->GetRequest());
	const Request&			request = client->GetRequest();
	std::string				path_to_target = _GetPathToTarget(request, location);
	Response				response;
	struct stat				buff;

	if (_CheckError(response, client, virtual_server, buff, path_to_target))
		_SetErrorPage(response, location, virtual_server, request);
	else
		_DefaultHandler(response, client, virtual_server, buff, path_to_target, location);

	try {
		if (request.GetHeader("connection") == "close")
			response.AddHeader("Connection", "Close");
		else
			throw std::out_of_range("");
	}
	catch (std::out_of_range&) {
		response.AddHeader("Connection", "Keep-alive");
	}
	client->SetResponse(response);
	client->GenerateResponse();
	client->NextStage();
}

void	WebServ::SendResponse(Client* client) {
	const Response&		response = client->GetResponse();
	const Request&		request = client->GetRequest();

	client->SendResponse();
	if (response.GetBuffer().empty()) {
		try {
			if (request.GetHeader("connection") == "close")
				client->SetStage(close_connection_);
			else
				throw std::out_of_range("");
		}
		catch (std::out_of_range&) {
			client->SetStage(read_request_);
		}

		client->ClearResponse();
		client->ClearRequest();
		client->ClearBuffer();
	}
}
