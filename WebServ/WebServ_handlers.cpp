/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_handlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:06:21 by imicah            #+#    #+#             */
/*   Updated: 2020/12/19 21:27:37 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

void	WebServ::ReadRequest(Client *client) {
	Request*	request = client->GetRequest();
	char		buff[1025];
	int 		bytes;

	bytes = recv(client->GetSocket(), buff, 1024, 0);
	buff[bytes] = 0;
	if (bytes == 0)
		client->SetStage(close_connection_);
	else {
		request->AddToBuffer(buff);
		if (bytes < 1024) client->NextStage();
	}
}

void	WebServ::SendResponse(Client* client) {
	Response*		response = client->GetResponse();
	Request*		request = client->GetRequest();

	client->SendResponse();
	if (!response->GetBuffer().size()) {
		try {
			if (request->GetHeader("connection") == "close")
				client->SetStage(close_connection_);
			else
				throw std::out_of_range("");
		}
		catch (std::out_of_range&) {
			client->SetStage(read_request_);
		}

		client->ClearResponse();
		client->ClearRequest();
	}
}

bool	WebServ::_CheckError(Client* client, VirtualServer* virtual_server, Location* location, struct stat& buff,
																						std::string& path_to_target) {
	Response*				response = client->GetResponse();
	Request*				request = client->GetRequest();
	int 					fd;

	if (stat(path_to_target.c_str(), &buff) == -1 || !location) {
		response->SetStatusCode("404");
		return (true);
	}
	if (S_ISDIR(buff.st_mode) && request->GetTarget().back() != '/') {
		response->SetStatusCode("301");
		response->AddHeader("Location", "http://" + virtual_server->GetIp() + ":" + virtual_server->GetPort() + request->GetTarget() + "/");
		return (true);
	}
	else if (!location->IsAllowMethod(request->GetMethod())) {
		response->SetStatusCode("405");
		return (true);
	}
	if (S_ISDIR(buff.st_mode) && (fd = open((path_to_target + location->GetIndex()).c_str(), O_RDONLY)) != -1) {
		path_to_target.append(location->GetIndex());
		stat(path_to_target.c_str(), &buff);
		close(fd);
	}
	else if (S_ISDIR(buff.st_mode) && !location->GetAutoindex()) {
		response->SetStatusCode("403");
		return (true);
	}
	else if (request->GetMethod() == "POST" && S_ISDIR(buff.st_mode)) {
		response->SetStatusCode("403");
		return (true);
	}
	else if (request->GetMethod() == "POST") {
		response->SetStatusCode("405");
		return (true);
	}
	return (false);
}

void	WebServ::_SetErrorPage(Client* client, Location* location, VirtualServer* virtual_server) {
	Response*		response = client->GetResponse();
	std::string		path_to_target;

	try {
		path_to_target.append(location->GetPath() + virtual_server->GetErrorPage(response->GetStatusCode()));
		response->AddHeader("Location", "http://" + virtual_server->GetIp() + ":" + virtual_server->GetPort() + path_to_target);
		response->SetStatusCode("302");
	}
	catch (std::exception&)
		{ response->SetBody(_GenerateErrorPage(response->GetStatusCode())); }
}

void	WebServ::_GetHeadMethodHandler(Client* client, Location* location, struct stat& buff, std::string& path_to_target) {
	Request*			request = client->GetRequest();
	Response*			response = client->GetResponse();
	bytes				body;
	struct timeval		tv;

	if (S_ISREG(buff.st_mode) || S_ISLNK(buff.st_mode)) {
		body = ft_getfile(path_to_target.c_str());
#ifdef __linux__
		tv.tv_sec = buff.st_mtim.tv_sec;
		tv.tv_usec = buff.st_mtim.tv_nsec;
#else
		tv.tv_sec = buff.st_mtimespec.tv_sec;
		tv.tv_usec = buff.st_mtimespec.tv_nsec;
#endif
		response->AddHeader("Last-modified", ft_getdate(tv));
	}
	else if (S_ISDIR(buff.st_mode) && location->GetAutoindex())
		body = _AutoindexGenerate(request, path_to_target);

	response->SetStatusCode("200");
	if (request->GetMethod() == "GET")
		response->SetBody(body);
}

void	WebServ::GenerateResponse(Client *client) {
	VirtualServer*		virtual_server = _GetVirtualServer(client);
	Location*			location = virtual_server->GetLocation(client->GetRequest());
	Request*			request = client->GetRequest();
	std::string			path_to_target = (location) ? _GetPathToTarget(request, location) : "";
	Response*			response = client->GetResponse();
	struct stat			buff;

	if (_CheckError(client, virtual_server, location, buff, path_to_target))
		_SetErrorPage(client, location, virtual_server);
	else if (request->GetMethod() == "GET" || request->GetMethod() == "HEAD")
		_GetHeadMethodHandler(client, location, buff, path_to_target);
//	else if (request->GetMethod() == "PUT")
//		_PutMethodHanler(client, location, buff, path_to_target);

	try {
		if (request->GetHeader("connection") == "close")
			response->AddHeader("Connection", "Close");
		else
			throw std::out_of_range("");
	}
	catch (std::out_of_range&) {
		response->AddHeader("Connection", "Keep-alive");
	}
	client->GenerateResponse();
	client->NextStage();
}
