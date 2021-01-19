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

#include "WebServ.hpp"

void	WebServ::readRequest(Client* client) {
	HttpRequest*	request = client->getRequest();
	int				size_buff = (request->getChunkSize() == -1 ? 2048 : request->getChunkSize());
	char			buff[size_buff + 1];
	int 			read_bytes;

	read_bytes = recv(client->getSocket(), buff, size_buff, 0);
	buff[read_bytes] = 0;
	try {
		if (read_bytes > 0) {
			client->setNewConnectionTime();
			request->addDataToRequest(buff, read_bytes);
			if (request->getStage() == completed)
				client->setStage(generate_response);
		}
		else
			client->setStage(close_connection);
	}
	catch (std::string& status_code) {
		VirtualServer*	virtual_server = getVirtualServer(client);
		Location*		location = virtual_server->getLocation(request);

		client->getResponse()->setStatusCode(status_code);
		setErrorPage(client, location, virtual_server);
		client->getResponse()->addHeader("Connection", "close");
		client->generateResponse();
		client->setStage(send_response);
	}
}

void	WebServ::sendResponse(Client* client) {
	HttpResponse*		response = client->getResponse();

	client->sendResponse();
	if (response->getBuffer().empty()) {
		if (response->findHeader("Connection") && response->getHeader("Connection") == "close")
			client->setStage(close_connection);
		else
			client->setStage(parsing_request);
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

void	WebServ::DefaultHandler(Client* client, Location* location, VirtualServer* virtual_server,  // TODO убрать virtual_server если в конце будет не нужен
																		t_stat* info, std::string& path_to_target) {
	HttpRequest*		request = client->getRequest();
	HttpResponse*		response = client->getResponse();
	struct timeval		tv;
	bytes				body;

	response->setStatusCode("200");
	if (S_ISDIR(info->info.st_mode) && !location->getAutoindex())
		response->setStatusCode("404");
	else if ((S_ISREG(info->info.st_mode) || S_ISLNK(info->info.st_mode))) {
		if (request->getMethod() == "GET")
			response->setBody(ft_getfile(path_to_target.c_str()));
#ifdef __linux__
		tv.tv_sec = info->info.st_mtim.tv_sec;
		tv.tv_usec = info->info.st_mtim.tv_nsec;
#else
		tv.tv_sec = info->info.st_mtimespec.tv_sec;
		tv.tv_usec = info->info.st_mtimespec.tv_nsec;
#endif
		response->addHeader("Last-modified", ft_getdate(tv));
	}
	else if (S_ISDIR(info->info.st_mode) && location->getAutoindex() && request->getMethod() == "GET")
		response->setBody(autoindexGenerate(request, path_to_target));
}

void	WebServ::putMethodHandler(Client* client, Location* location, VirtualServer* virtual_server, t_stat* info, // TODO убрать virtual_server если в конце будет не нужен
																						std::string& path_to_target) {
	int 			fd;
	HttpRequest*	request = client->getRequest();
	HttpResponse*	response = client->getResponse();

	// TODO проверить права на запись - иначе 403
	if (location->getLimitClientBodySize() < request->getBody().size())
		response->setStatusCode("413");
	else if (S_ISDIR(info->info.st_mode) || (fd = open(path_to_target.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
		response->setStatusCode("404");
	else {
		write(fd, request->getBody().c_str(), request->getBody().size());
		(info->exists == -1) ? response->setStatusCode("201") : response->setStatusCode("200");
	}
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

std::string WebServ::isErrorRequest(Location* location, t_stat& info, Client* client) {
	HttpRequest*		request = client->getRequest();

	if (!location || (info.exists == -1 && (request->getMethod() != "PUT" && request->getMethod() != "POST")))
		return ("404");
	else if (!checkAuth(client, location->getRoot()))
		return ("401");
	else if (!location->isAllowMethod(request->getMethod()))
		return ("405");
	else if (request->getBody().size() > location->getLimitClientBodySize())
		return ("413");
	return ("");
}

void	WebServ::generateResponse(Client *client) {
	VirtualServer*		virtual_server = getVirtualServer(client);
	Location*			location = virtual_server->getLocation(client->getRequest());
	HttpRequest*		request = client->getRequest();
	HttpResponse*		response = client->getResponse();
	std::string			path_to_target = (location) ? getPathToTarget(request, location) : "";
	t_stat				info = {};
	std::string			error_code;
	int 				fd;

	info.exists = stat(path_to_target.c_str(), &info.info);
	if (!(error_code = isErrorRequest(location, info, client)).empty())
		response->setStatusCode(error_code);
	else {
		if (S_ISDIR(info.info.st_mode) && (fd = open((path_to_target + "/" + location->getIndex()).c_str(), O_RDONLY)) != -1) {
			path_to_target.append("/" + location->getIndex());
			info.exists = stat(path_to_target.c_str(), &info.info);
			close(fd);
		}
		if (location->findCgi(path_to_target) || (request->getMethod() == "POST" && S_ISDIR(info.info.st_mode) && location->findCgi(".bla")))
			cgiHandler(client, path_to_target, location);
		else if (request->getMethod() == "GET" || request->getMethod() == "HEAD")
			DefaultHandler(client, location, virtual_server, &info, path_to_target);
		else if (request->getMethod() == "PUT")
			putMethodHandler(client, location, virtual_server, &info, path_to_target);
		else
			response->setStatusCode("405");
	}

	if (request->findHeader("connection") && request->getHeader("connection") == "close")
		response->addHeader("Connection", "close");
	if (isErrorStatus(response->getStatusCode()))
		setErrorPage(client, location, virtual_server);
	client->generateResponse();
	client->setStage(send_response);
}
