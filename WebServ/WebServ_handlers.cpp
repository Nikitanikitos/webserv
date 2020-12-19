/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_handlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:06:21 by imicah            #+#    #+#             */
/*   Updated: 2020/12/19 23:18:38 by nikita           ###   ########.fr       */
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
		if (request->FindHeader("connection") && request->GetHeader("connection") == "close")
			client->SetStage(close_connection_);
		else
			client->SetStage(read_request_);
	}
	client->ClearResponse();
	client->ClearRequest();
}

void	WebServ::_SetErrorPage(Client* client, Location* location, VirtualServer* virtual_server) {
	Response*		response = client->GetResponse();
	std::string		path_to_target;

	if (virtual_server->FindErrorPage(response->GetStatusCode())) {
		path_to_target.append(location->GetPath() + virtual_server->GetErrorPage(response->GetStatusCode()));
		response->AddHeader("Location", "http://" + virtual_server->GetIp() + ":" + virtual_server->GetPort() + path_to_target);
		response->SetStatusCode("302");
	}
	else
		{ response->SetBody(_GenerateErrorPage(response->GetStatusCode())); }
}

void	WebServ::_GetHeadMethodHandler(Client* client, Location* location, VirtualServer* virtual_server,
																	struct stat& buff, std::string& path_to_target) {
	Request*			request = client->GetRequest();
	Response*			response = client->GetResponse();
	bytes				body;
	struct timeval		tv;
	int 				fd;

	response->SetStatusCode("200");
	if (!buff.st_dev)
		response->SetStatusCode("404");
	else if (S_ISDIR(buff.st_mode) && (fd = open((path_to_target + location->GetIndex()).c_str(), O_RDONLY)) != -1) {
		path_to_target.append(location->GetIndex());
		stat(path_to_target.c_str(), &buff);
		close(fd);
	}
	else if (S_ISDIR(buff.st_mode) && !location->GetAutoindex())
		response->SetStatusCode("403");
	else if (S_ISREG(buff.st_mode) || S_ISLNK(buff.st_mode)) {
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

	if (request->GetMethod() == "GET" && response->GetStatusCode() == "200")
		response->SetBody(body);
}

void WebServ::_PutMethodHandler(Client* client, Location* location, VirtualServer* virtual_server, struct stat& buff,
		std::string& path_to_target) {
	int 		fd;
	Request*	request = client->GetRequest();
	Response*	response = client->GetResponse();

	fd = 0;
	if (S_ISDIR(buff.st_mode))
		response->SetStatusCode("404");
	else if (!buff.st_dev) {
		if ((fd = open(path_to_target.c_str(), O_WRONLY | O_CREAT)) > 0)
			response->SetStatusCode("201");
		else
			response->SetStatusCode("404");
	}
	else {
		fd = open(path_to_target.c_str(), O_WRONLY | O_EXCL);
		response->SetStatusCode("200");
	}
	if (fd > 0)
		write(fd, request->GetBody().c_str(), request->GetBody().size());
}

void	WebServ::GenerateResponse(Client *client) {
	VirtualServer*		virtual_server = _GetVirtualServer(client);
	Location*			location = virtual_server->GetLocation(client->GetRequest());
	Request*			request = client->GetRequest();
	std::string			path_to_target = (location) ? _GetPathToTarget(request, location) : "";
	Response*			response = client->GetResponse();
	struct stat			buff;

	stat(path_to_target.c_str(), &buff);
	if (!location)
		response->SetStatusCode("404");
	else if (!location->IsAllowMethod(request->GetMethod()))
		response->SetStatusCode("405");
	else if (S_ISDIR(buff.st_mode) && request->GetTarget().back() != '/') {
		response->SetStatusCode("301");
		response->AddHeader("Location", "http://" + virtual_server->GetIp() + ":" + virtual_server->GetPort() + request->GetTarget() + "/");
	}
	else if (request->GetMethod() == "GET" || request->GetMethod() == "HEAD")
		_GetHeadMethodHandler(client, location, virtual_server, buff, path_to_target);
	else if (request->GetMethod() == "PUT")
		_PutMethodHandler(client, location, virtual_server, buff, path_to_target);

	if (request->FindHeader("connection") && request->GetHeader("connection") == "close")
		response->AddHeader("Connection", "Close");
	if (_IsErrorStatus(response->GetStatusCode()))
		_SetErrorPage(client, location, virtual_server);
	client->GenerateResponse();
	client->NextStage();
}

bool	WebServ::_IsErrorStatus(const std::string& status) {
	const std::string	status_code[4] = {"400", "403", "404", "405"};

	for (int i = 0; i < 4; ++i)
		if (status == status_code[i])
			return (true);
	return (false);
}
