/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_default_handler.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 23:04:25 by imicah            #+#    #+#             */
/*   Updated: 2020/12/07 15:55:07 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

void WebServ::_default_handler(HttpObject *http_object, const VirtualServer& virtual_server, const Location& location) {
	const Request&		request = http_object->get_request();
	const std::string&	path_to_target = _get_path_to_target(request, location);
	struct stat			buff;

	if (stat(path_to_target.c_str(), &buff) == -1)
		throw RequestException("404", "Not Found", virtual_server.get_error_page("404"));
	else if (S_ISDIR(buff.st_mode) && path_to_target.back() != '/')
		throw Request301Redirect("http://" + request.get_host() + ":" + request.get_port() + "/" + request.get_target() + "/");
	else if (!location.is_allow_method(request.get_method()))
		throw RequestException("405", "Method Not Allowed", virtual_server.get_error_page("405"));
	else if (S_ISDIR(buff.st_mode) && !location.is_autoindex())
		throw RequestException("403", "Forbidden", virtual_server.get_error_page("403"));

	if (request.get_method() == "POST")
		_POST_method_handler(request, &buff, virtual_server);
	else
		_GET_HEAD_methods_handler(http_object, &buff, location);
}

void		WebServ::_POST_method_handler(const Request& request, struct stat* buff,
																				const VirtualServer& virtual_server) {
	if (S_ISDIR(buff->st_mode))
		throw RequestException("403", "Forbidden", virtual_server.get_error_page("403"));
	else
		throw RequestException("405", "Method Not Allowed", virtual_server.get_error_page("405"));
}

void		WebServ::_GET_HEAD_methods_handler(HttpObject *http_object, struct stat* buff, const Location& location) {
	const Request&		request = http_object->get_request();
	const std::string&	path_to_target = _get_path_to_target(request, location);

	if (S_ISREG(buff->st_mode) || S_ISLNK(buff->st_mode))
		http_object->set_response(_static_file_handler(request, path_to_target));
	else if (S_ISDIR(buff->st_mode) && location.is_autoindex())
		http_object->set_response(_autoindex_handler(request, path_to_target));
}

Response WebServ::_static_file_handler(const Request& request, const std::string& path_to_file) {
	Response				response;
	const std::string		body_response = ft_getfile(path_to_file.c_str());
	char					last_modified[80];

	response.set_status_code("200");
	response.add_header("Content-Length", std::to_string(body_response.length()));
	response.add_header("Last-modified", last_modified);
	response.add_header("Connection", "Close");
	if (request.get_method() == "GET")
		response.set_body(body_response);
	return (response);
}

Response WebServ::_autoindex_handler(const Request& request, const std::string& path_to_target) {
	Response			response;
	std::string 		body_response;

	body_response = _autoindex_generate(request, path_to_target);

	response.set_status_code("200");
	response.add_header("Content-Length", std::to_string(body_response.length()));
	response.add_header("Connection", "Close");
	if (request.get_method() == "GET")
		response.set_body(body_response);
	return (response);
}

std::string	WebServ::_autoindex_generate(const Request& request, const std::string& path_to_target) {
	std::string 		body_response;
	DIR*				directory;
	dirent*				current_file;

	directory = opendir(path_to_target.c_str());
	current_file = readdir(directory);
	current_file = readdir(directory);
	body_response.append("<html><head><title>Index of " + request.get_target() + "</title></head><body>"
								 "<h1>Index of "  + request.get_target() + "</h1><hr><pre><a href=\"../\">../</a>");
	while ((current_file = readdir(directory)) != nullptr) {
		const std::string&	file(current_file->d_name);
		body_response.append("<a href \"" + file + "\">" + file + "</a>");
	}
	body_response.append("</pre><hr></body></html>");
	return (body_response);
}
