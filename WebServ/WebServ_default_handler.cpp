/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_default_handler.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 23:04:25 by imicah            #+#    #+#             */
/*   Updated: 2020/12/12 08:05:39 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

void WebServ::_DefaultHandler(Client *client, const VirtualServer& virtual_server, const Location& location) {
	Request&			request = client->get_request();
	const std::string&	path_to_target = _GetPathToTarget(request, location);
	struct stat			buff;

	if (stat(path_to_target.c_str(), &buff) == -1)
		throw ResponseException("404", "Not Found", virtual_server.GetErrorPage("404"));
	else if (S_ISDIR(buff.st_mode) && request.GetTarget().back() != '/')
		throw Request301Redirect("http://" + request.GetHost() + ":" + request.GetPort() + "/" + request.GetTarget() + "/");
	else if (!location.is_allow_method(request.GetMethod()))
		throw ResponseException("405", "Method Not Allowed", virtual_server.GetErrorPage("405"));
	else if (S_ISDIR(buff.st_mode) && !location.get_autoindex())
		throw ResponseException("403", "Forbidden", virtual_server.GetErrorPage("403"));

	if (request.GetMethod() == "POST")
		_PostMethodHandler(request, &buff, virtual_server);
	else
		_GetHeadMethodsHandler(client, &buff, location);
}

void		WebServ::_PostMethodHandler(const Request& request, struct stat* buff,
										const VirtualServer& virtual_server) {
	if (S_ISDIR(buff->st_mode))
		throw ResponseException("403", "Forbidden", virtual_server.GetErrorPage("403"));
	else
		throw ResponseException("405", "Method Not Allowed", virtual_server.GetErrorPage("405"));
}

void		WebServ::_GetHeadMethodsHandler(Client *http_object, struct stat* buff, const Location& location) {
	Request&			request = http_object->get_request();
	const std::string&	path_to_target = _GetPathToTarget(request, location);

	if (S_ISREG(buff->st_mode) || S_ISLNK(buff->st_mode))
		http_object->set_response(_StaticFileHandler(request, path_to_target));
	else if (S_ISDIR(buff->st_mode) && location.get_autoindex())
		http_object->set_response(_AutoindexHandler(request, path_to_target));
}

Response WebServ::_StaticFileHandler(const Request& request, const std::string& path_to_file) {
	Response				response;
	const std::string		body_response = ft_getfile(path_to_file.c_str());
	char					last_modified[80];

	response.SetStatusCode("200");
	response.AddHeader("Content-Length", std::to_string(body_response.length()));
	response.AddHeader("Last-modified", last_modified);

	if (response.GetHeader("Connection") == "close")
		response.AddHeader("Connection", "Close");
	else
		response.AddHeader("Connection", "Keep-alive");

	if (request.GetMethod() == "GET")
		response.SetBody(body_response);
	return (response);
}

Response WebServ::_AutoindexHandler(const Request& request, const std::string& path_to_target) {
	Response			response;
	std::string 		body_response;

	body_response = _AutoindexGenerate(request, path_to_target);

	response.SetStatusCode("200");
	response.AddHeader("Content-Length", std::to_string(body_response.length()));
	response.AddHeader("Connection", "Close");
	if (request.GetMethod() == "GET")
		response.SetBody(body_response);
	return (response);
}

std::string	WebServ::_AutoindexGenerate(const Request& request, const std::string& path_to_target) {
	std::string 		body_response;
	DIR*				directory;
	dirent*				current_file;

	directory = opendir(path_to_target.c_str());
	current_file = readdir(directory);
	current_file = readdir(directory);
	body_response.append("<html><head><title>Index of " + request.GetTarget() + "</title></head><body>"
								 "<h1>Index of " + request.GetTarget() + "</h1><hr><pre><a href=\"../\">../</a>");
	while ((current_file = readdir(directory)) != nullptr) {
		const std::string&	file(current_file->d_name);
		body_response.append("<a href \"" + file + "\">" + file + "</a>");
	}
	body_response.append("</pre><hr></body></html>");
	return (body_response);
}
