/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_handlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:06:21 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 20:20:38 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "WebServ.hpp"

void	WebServ::readRequest(Client *client) {
	HttpRequest*	request = client->getRequest();
	HttpResponse*	response = client->getResponse();
	char			buff[1025];
	int 			read_bytes;
	bytes			request_data;

	read_bytes = recv(client->getSocket(), buff, 1024, 0);
	buff[read_bytes] = 0;
	request->addToBuffer(buff, read_bytes);
	while (request->getBuffer().size()) {
		request_data = request->getRequestData();
		switch (request->getStage()) {
			case parsing_first_line:
				HandlerHttpObject::parsingFirstLine(request, response, request_data.c_str());
				break;
			case parsing_headers:
				if (!request_data.size())
					HandlerHttpObject::endOfHeaders(request, response);
				else if (!HandlerHttpObject::parseHeader(request, request_data.c_str())) {
					response->setStatusCode("400");
					request->setStage(bad_request);
				}
				break;
			case parsing_body:
				request->addToBody(request_data);
				if (request->getBody().size() >= ft_atoi(request->getHeader("content-length").c_str())) {
					request->trimBody(0);
					request->setStage(completed);
				}
		}
		if (request->getStage() == completed)
			client->setStage(generate_response_);
		else if (request->getStage() == bad_request) {
			VirtualServer*	virtual_server = getVirtualServer(client);
			Location*		location = virtual_server->getLocation(request);

			setErrorPage(client, location, virtual_server);
			client->generateResponse();
			client->setStage(send_response_);
		}
	}
}

void	WebServ::sendResponse(Client* client) {
	HttpResponse*		response = client->getResponse();
	HttpRequest*		request = client->getRequest();

	client->sendResponse();
	if (!response->getBuffer().size()) {
		if (request->findHeader("connection") && request->getHeader("connection") == "close")
			client->setStage(close_connection_);
		else
			client->setStage(parsing_request_);
		client->clearResponse();
		client->clearRequest();
	}
}

void	WebServ::setErrorPage(Client* client, Location* location, VirtualServer* virtual_server) {
	HttpResponse*	response = client->getResponse();
	std::string		path_to_target;

	if (virtual_server->findErrorPage(response->getStatusCode())) {
		path_to_target.append(location->getPath() + virtual_server->getErrorPage(response->getStatusCode()));
		response->addHeader("Location", "http://" + client->getHost() + ":" + client->getPort() + path_to_target);
		response->setStatusCode("302");
	}
	else
		{ response->setBody(generateErrorPage(response->getStatusCode())); }
}

void	WebServ::getHeadMethodHandler(Client* client, Location* location, VirtualServer* virtual_server,
									  t_stat* info, std::string& path_to_target) {
	HttpRequest*		request = client->getRequest();
	HttpResponse*		response = client->getResponse();
	bytes				body;
	struct timeval		tv;
	int 				fd;

	response->setStatusCode("200");
	if (info->exists == -1)
		response->setStatusCode("404");
	else if (S_ISDIR(info->info.st_mode) && (fd = open((path_to_target + location->getIndex()).c_str(), O_RDONLY)) != -1) {
		path_to_target.append(location->getIndex());
		info->exists = stat(path_to_target.c_str(), &info->info);
		close(fd);
	}
	else if (S_ISDIR(info->info.st_mode) && !location->getAutoindex())
		response->setStatusCode("403");
	if (info->exists != -1 && (S_ISREG(info->info.st_mode) || S_ISLNK(info->info.st_mode))) {
		body = ft_getfile(path_to_target.c_str());
#ifdef __linux__
		tv.tv_sec = info->info.st_mtim.tv_sec;
		tv.tv_usec = info->info.st_mtim.tv_nsec;
#else
		tv.tv_sec = info->info.st_mtimespec.tv_sec;
		tv.tv_usec = info->info.st_mtimespec.tv_nsec;
#endif
		response->addHeader("Last-modified", ft_getdate(tv));
	}
	else if (info->exists != -1 && S_ISDIR(info->info.st_mode) && location->getAutoindex())
		body = autoindexGenerate(request, path_to_target);

	if (request->getMethod() == "GET" && response->getStatusCode() == "200")
		response->setBody(body);
}

void WebServ::putMethodHandler(Client* client, Location* location, VirtualServer* virtual_server, t_stat* info,
							   std::string& path_to_target) {
	int 			fd;
	HttpRequest*	request = client->getRequest();
	HttpResponse*	response = client->getResponse();

	// TODO проверить права на запись - иначе 403
	fd = 0;
	if (S_ISDIR(info->info.st_mode))
		response->setStatusCode("404");
	else if (info->exists == -1) {
		if ((fd = open(path_to_target.c_str(), O_WRONLY | O_CREAT, 0666)) > 0)
			response->setStatusCode("201");
		else
			response->setStatusCode("404");
	}
	else {
		fd = open(path_to_target.c_str(), O_WRONLY | O_EXCL | O_TRUNC);
		response->setStatusCode("200");
	}
	if (fd > 0)
		write(fd, request->getBody().c_str(), request->getBody().size());
}

void	WebServ::generateResponse(Client *client) {
	VirtualServer*		virtual_server = getVirtualServer(client);
	Location*			location = virtual_server->getLocation(client->getRequest());
	HttpRequest*		request = client->getRequest();
	std::string			path_to_target = (location) ? getPathToTarget(request, location) : "";
	HttpResponse*		response = client->getResponse();
	t_stat				info;

	info.exists = stat(path_to_target.c_str(), &info.info);
	if (!location)
		response->setStatusCode("404");
	else if (!location->isAllowMethod(request->getMethod()))
		response->setStatusCode("405");
	else if (info.exists != -1 && S_ISDIR(info.info.st_mode) && request->getTarget()[request->getTarget().size() - 1] != '/') {
		response->setStatusCode("301");
		response->addHeader("Location", "http://" + client->getHost() + ":" + client->getPort() +
										request->getTarget() + "/");
	}
	else if (request->getMethod() == "GET" || request->getMethod() == "HEAD")
		getHeadMethodHandler(client, location, virtual_server, &info, path_to_target);
	else if (request->getMethod() == "PUT")
		putMethodHandler(client, location, virtual_server, &info, path_to_target);

	if (request->findHeader("connection") && request->getHeader("connection") == "close")
		response->addHeader("Connection", "Close");
	if (HandlerHttpObject::isErrorStatus(response->getStatusCode()))
		setErrorPage(client, location, virtual_server);
	client->generateResponse();
	client->setStage(send_response_);
}
