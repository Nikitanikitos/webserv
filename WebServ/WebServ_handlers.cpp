/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_handlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:06:21 by imicah            #+#    #+#             */
/*   Updated: 2020/12/24 18:09:42 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "WebServ.hpp"

void	WebServ::readRequest(Client* client) {
	HttpRequest*	request = client->getRequest();
	char			buff[1025];
	int 			read_bytes;

	read_bytes = recv(client->getSocket(), buff, 1024, 0);
	buff[read_bytes] = 0;
	try {
		if (read_bytes > 0) {
			client->setNewConnectionTime();
			request->addDataToRequest(bytes(buff, read_bytes));
			if (request->getStage() == completed)
				client->setStage(generate_response_);
		}
		else
			client->setStage(close_connection_);
	}
	catch (std::string& status_code) {
		VirtualServer*	virtual_server = getVirtualServer(client);
		Location*		location = virtual_server->getLocation(request);

		client->getResponse()->setStatusCode(status_code);
		setErrorPage(client, location, virtual_server);
		client->getResponse()->addHeader("Connection", "close");
		client->generateResponse();
		client->setStage(send_response_);
	}
}

void	WebServ::sendResponse(Client* client) {
	HttpResponse*		response = client->getResponse();

	client->sendResponse();
	if (response->getBuffer().empty()) {
		if (response->findHeader("Connection") && response->getHeader("Connection") == "close")
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

void	WebServ::DefaultHandler(Client* client, Location* location, VirtualServer* virtual_server,
								t_stat* info, std::string& path_to_target) {
	HttpRequest*		request = client->getRequest();
	HttpResponse*		response = client->getResponse();
	bytes				body;
	struct timeval		tv;
	int 				fd;

	response->setStatusCode("200");
	if (info->exists == -1)
		response->setStatusCode("404");
	else if (S_ISDIR(info->info.st_mode) && !location->getIndex().empty() && (fd = open((path_to_target + location->getIndex()).c_str(), O_RDONLY)) != -1) {
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

void	WebServ::putMethodHandler(Client* client, Location* location, VirtualServer* virtual_server, t_stat* info, // TODO убрать virtual_server если в конце будет не нужен
							   std::string& path_to_target) {
	int 			fd = 0;
	HttpRequest*	request = client->getRequest();
	HttpResponse*	response = client->getResponse();

	// TODO проверить права на запись - иначе 403
	if (location->getLimitClientBodySize() < request->getBody().size())
		response->setStatusCode("413");
	else if (S_ISDIR(info->info.st_mode))
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

bool	isErrorStatus(const std::string& status) {
	const std::string	status_code[count_error_pages] = {"400", "401", "403", "404", "405", "411", "413"};

	for (int i = 0; i < count_error_pages; ++i)
		if (status == status_code[i]) return (true);
	return (false);
}

void	WebServ::getInfoOutHtaccess(int fd, std::string& realm, std::string& path_to_htpasswd) {
	std::string		line;

	while (ft_getline(fd, line) > 0) {
		if (line.find("AuthName") != std::string::npos)
			realm = line.substr(line.find('\"'));
		else if (line.find("AuthUserFile") != std::string::npos)
			path_to_htpasswd = line.substr(line.find(' ') + 1);
	}
}

bool	WebServ::checkValidAuth(const std::string& login_password, const std::string& path_to_htpasswd) {
	int						fd;
	std::string				line;
	const std::string		decode_login_password = ft_decode64base(login_password.substr(login_password.find(' ') + 1));

	if ((fd = open(path_to_htpasswd.c_str(), O_RDONLY)) > 0) {
		while (ft_getline(fd, line) > 0)
			if (line == decode_login_password) return (true);
	}
	return (false);
}

bool	WebServ::checkAuth(Client* client, const std::string& root) {
	int					fd;
	bool				result;
	HttpRequest*		request = client->getRequest();
	HttpResponse*		response = client->getResponse();
	std::string			realm;
	std::string			path_to_htpasswd;

	result = true;
	if ((fd = open((root + "/.htaccess").c_str(), O_RDONLY)) != -1) {
		getInfoOutHtaccess(fd, realm, path_to_htpasswd);
		if (request->findHeader("authorization") && checkValidAuth(request->getHeader("authorization"), path_to_htpasswd))
			result = true;
		else {
			result = false;
			response->addHeader("WWW-Authenticate", "Basic realm=" + realm);
		}
		close(fd);
	}
	return (result);
}

void	WebServ::generateResponse(Client *client) {
	VirtualServer*		virtual_server = getVirtualServer(client);
	Location*			location = virtual_server->getLocation(client->getRequest());
	HttpRequest*		request = client->getRequest();
	HttpResponse*		response = client->getResponse();
	std::string			path_to_target = (location) ? getPathToTarget(request, location) : "";
	t_stat				info;

	info.exists = stat(path_to_target.c_str(), &info.info);
	if (!location)
		response->setStatusCode("404");
	else if (!checkAuth(client, location->getRoot()))
		response->setStatusCode("401");
	else if (!location->isAllowMethod(request->getMethod()))
		response->setStatusCode("405");
	else if (info.exists != -1 && S_ISDIR(info.info.st_mode) && request->getTarget()[request->getTarget().size() - 1] != '/') {
		response->setStatusCode("301");
		response->addHeader("Location", "http://" + client->getHost() + ":" + client->getPort() + request->getTarget() + "/");
	}
	else// if (isCgi())
		cgiHandler(client, path_to_target);
//	else if (request->getMethod() == "GET" || request->getMethod() == "HEAD")
//		DefaultHandler(client, location, virtual_server, &info, path_to_target);
//	else if (request->getMethod() == "PUT")
//		putMethodHandler(client, location, virtual_server, &info, path_to_target);
//	else
//		response->setStatusCode("405");

	if (request->findHeader("connection") && request->getHeader("connection") == "close")
		response->addHeader("Connection", "Close");
	if (isErrorStatus(response->getStatusCode()))
		setErrorPage(client, location, virtual_server);
	client->generateResponse();
	client->setStage(send_response_);
}
