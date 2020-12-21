/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_handlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:06:21 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 13:00:35 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

void	WebServ::readRequest(Client *client) {
	HttpRequest*	request = client->getRequest();
	char		buff[1025];
	int 		bytes;

	bytes = recv(client->getSocket(), buff, 1024, 0);
	buff[bytes] = 0;
	if (bytes <= 0)
		client->setStage(close_connection_);
	else {
		request->addToBuffer(buff, bytes);
		if (bytes < 1024) client->nextStage();
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
			client->setStage(read_request_);
	}
	client->clearResponse();
	client->clearRequest();
}

void	WebServ::setErrorPage(Client* client, Location* location, VirtualServer* virtual_server) {
	HttpResponse*		response = client->getResponse();
	std::string		path_to_target;

	if (virtual_server->findErrorPage(response->getStatusCode())) {
		path_to_target.append(location->getPath() + virtual_server->getErrorPage(response->getStatusCode()));
		response->addHeader("Location",
							"http://" + virtual_server->getHost() + ":" + virtual_server->getPort() + path_to_target);
		response->setStatusCode("302");
	}
	else
		{ response->SetBody(generateErrorPage(response->getStatusCode())); }
}

void	WebServ::getHeadMethodHandler(Client* client, Location* location, VirtualServer* virtual_server,
									  struct stat* buff, std::string& path_to_target) {
	HttpRequest*			request = client->getRequest();
	HttpResponse*			response = client->getResponse();
	bytes				body;
	struct timeval		tv;
	int 				fd;

	response->setStatusCode("200");
	if (!buff->st_dev)
		response->setStatusCode("404");
	else if (S_ISDIR(buff->st_mode) && (fd = open((path_to_target + location->getIndex()).c_str(), O_RDONLY)) != -1) {
		path_to_target.append(location->getIndex());
		stat(path_to_target.c_str(), buff);
		close(fd);
	}
	else if (S_ISDIR(buff->st_mode) && !location->getAutoindex())
		response->setStatusCode("403");
	if (S_ISREG(buff->st_mode) || S_ISLNK(buff->st_mode)) {
		body = ft_getfile(path_to_target.c_str());
#ifdef __linux__
		tv.tv_sec = buff->st_mtim.tv_sec;
		tv.tv_usec = buff->st_mtim.tv_nsec;
#else
		tv.tv_sec = buff->st_mtimespec.tv_sec;
		tv.tv_usec = buff->st_mtimespec.tv_nsec;
#endif
		response->addHeader("Last-modified", ft_getdate(tv));
	}
	else if (S_ISDIR(buff->st_mode) && location->getAutoindex())
		body = autoindexGenerate(request, path_to_target);

	if (request->getMethod() == "GET" && response->getStatusCode() == "200")
		response->SetBody(body);
}

void WebServ::putMethodHandler(Client* client, Location* location, VirtualServer* virtual_server, struct stat* buff,
							   std::string& path_to_target) {
	int 		fd;
	HttpRequest*	request = client->getRequest();
	HttpResponse*	response = client->getResponse();

	fd = 0;
	if (!request->findHeader("content-length"))
		response->setStatusCode("411");
	else if (request->getHeader("content-length").size() > virtual_server->getLimitBodySize())
		response->setStatusCode("413");
	else if (S_ISDIR(buff->st_mode))
		response->setStatusCode("404");
	else if (!buff->st_dev) {
		if ((fd = open(path_to_target.c_str(), O_WRONLY | O_CREAT)) > 0)
			response->setStatusCode("201");
		else
			response->setStatusCode("404");
	}
	else {
		fd = open(path_to_target.c_str(), O_WRONLY | O_EXCL);
		response->setStatusCode("200");
	}
	if (fd > 0)
		write(fd, request->getBody().c_str(), request->getBody().size());
}

void	WebServ::generateResponse(Client *client) {
	VirtualServer*		virtual_server = getVirtualServer(client);
	Location*			location = virtual_server->getLocation(client->getRequest());
	HttpRequest*			request = client->getRequest();
	std::string			path_to_target = (location) ? getPathToTarget(request, location) : "";
	HttpResponse*			response = client->getResponse();
	struct stat			buff;

	stat(path_to_target.c_str(), &buff);
	if (!location)
		response->setStatusCode("404");
	else if (!location->isAllowMethod(request->getMethod()))
		response->setStatusCode("405");
	else if (S_ISDIR(buff.st_mode) && request->getTarget().back() != '/') {
		response->setStatusCode("301");
		response->addHeader("Location", "http://" + virtual_server->getHost() + ":" + virtual_server->getPort() +
										request->getTarget() + "/");
	}
	else if (request->getMethod() == "GET" || request->getMethod() == "HEAD")
		getHeadMethodHandler(client, location, virtual_server, &buff, path_to_target);
	else if (request->getMethod() == "PUT")
		putMethodHandler(client, location, virtual_server, &buff, path_to_target);

	if (request->findHeader("connection") && request->getHeader("connection") == "close")
		response->addHeader("Connection", "Close");
	if (isErrorStatus(response->getStatusCode()))
		setErrorPage(client, location, virtual_server);
	client->generateResponse();
	client->nextStage();
}

bool	WebServ::isErrorStatus(const std::string& status) {
	const std::string	status_code[4] = {"400", "403", "404", "405"};

	for (int i = 0; i < 4; ++i)
		if (status == status_code[i])
			return (true);
	return (false);
}
