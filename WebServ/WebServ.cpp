/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/12/03 22:51:00 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <fstream>
#include "WebServ.hpp"

WebServ::WebServ(const std::vector<VirtualServer>& list_virtual_servers) : _list_virtual_servers(list_virtual_servers) {
	FD_ZERO(&_set_of_vs_sockets);

	for (const VirtualServer& server : _list_virtual_servers)
		for (const int fd_socket : server.vs_sockets) {
			_vs_sockets.push_back(fd_socket);
			FD_SET(fd_socket, &_set_of_vs_sockets);
		}
	_create_workers();
}

WebServ::~WebServ() {
	std::pair<int, int>		worker;

	while (!_worker_queue.empty()){
		worker = _pop_worker();
		close(worker.first);
	}
}

void		WebServ::run_server() {
	while (true) {
		select(_vs_sockets.back() + 1, &_set_of_vs_sockets, nullptr, nullptr, nullptr);
		_get_accept_from_ready_sockets();
	}
}

void		WebServ::_serve_client(int client_socket) {
	Request			request;
	Location		location;
	VirtualServer	virtual_server;

	try {
		request = _get_request(client_socket);
		virtual_server = _get_virtual_server(request);
		location = virtual_server.get_location(request);
		switch (location.get_location_type()) {
			case _default:
				_default_handler(request, virtual_server, location, client_socket);
			case cgi:
				_cgi_handler(request, virtual_server, location, client_socket);
			case proxy:
				_proxy_handler(request, virtual_server, location, client_socket);
		}
	}
	catch (Request301Redirect& redirect_301) {
		redirect_301.send_response(client_socket);
	}
	catch (RequestException& request_error) {
		request_error.send_response(client_socket);
	}
}

const Request		WebServ::_get_request(int client_socket) {
	return (Request());
}

void		WebServ::_default_handler(Request& request, const VirtualServer& virtual_server, const Location& location,
																								int client_socket) {
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
		_post_method_handler(request, &buff, virtual_server);
	else
		_get_head_methods_handler(request, &buff, client_socket, location);
}

void
WebServ::_get_head_methods_handler(Request& request, struct stat* buff, int client_socket, const Location& location) {
	const std::string&	path_to_target = _get_path_to_target(request, location);

	if (S_ISREG(buff->st_mode) || S_ISLNK(buff->st_mode))
		_static_file_handler(request, path_to_target, client_socket);
	else if (S_ISDIR(buff->st_mode) && location.is_autoindex())
		_autoindex_handler(request, path_to_target, client_socket);
}

void WebServ::_post_method_handler(Request& request, struct stat* buff, const VirtualServer& virtual_server) {
	if (S_ISDIR(buff->st_mode))
		throw RequestException("403", "Forbidden", virtual_server.get_error_page("403"));
	else
		throw RequestException("405", "Method Not Allowed", virtual_server.get_error_page("405"));
}

void WebServ::_autoindex_handler(Request& request, const std::string& path_to_target, int client_socket) {
	Response			response;
	std::string 		body_response;
	DIR*				directory = opendir(path_to_target.c_str());
	dirent*				current_file;

	current_file = readdir(directory);
	current_file = readdir(directory);
	body_response.append("<html><head><title>Index of " + request.get_target() + "</title></head><body>"
						"<h1>Index of "  + request.get_target() + "</h1><hr><pre><a href=\"../\">../</a>");
	while ((current_file = readdir(directory)) != nullptr) {
		const std::string&	file(current_file->d_name);
		body_response.append("<a href \"" + file + "\">" + file + "</a>");
	}
	body_response.append("</pre><hr></body></html>");

	response.set_status_code("200");
	response.add_header("Content-Length", std::to_string(body_response.length()));
	response.set_body(body_response);
	response.send_response(client_socket);
}

void WebServ::_static_file_handler(Request& request, const std::string& path_to_file, int client_socket) {
	Response			response;
	std::ifstream		file(path_to_file);
	std::string			body_response;
	char				last_modified[80];

	response.set_status_code("200");
	getline(file, body_response, '\0');
	response.add_header("Content-Length", std::to_string(body_response.length()));
	response.add_header("Last-modified", last_modified);
	if (request.get_method() == "GET")
		response.set_body(body_response);

	response.send_response(client_socket);
}

const VirtualServer& WebServ::_get_virtual_server(const Request& request) const {
	bool					default_vs_flag;
	const VirtualServer		*default_vs;

	default_vs_flag = false;
	for (const auto & server : _list_virtual_servers) {
		for (const auto& port : server.get_ports()) {
			if (request.get_port() == port) {
				if (!default_vs_flag) {
					default_vs = &server;
					default_vs_flag = true;
				}
				for (const auto& server_name : server.get_server_names())
					if (request.get_header(HOST) == server_name) {
						default_vs = &server;
						break ;
					}
			}
		}
	}
	return (*default_vs);
}

std::string		WebServ::_get_path_to_target(const Request& request, const Location& location) {
	if (request.get_target() == location.get_path())
		return (location.get_root() + location.get_index());
	else
		return (location.get_root() + request.get_target().substr(location.get_path().length()));
}
