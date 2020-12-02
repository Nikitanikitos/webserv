/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/12/01 20:34:22 by imicah           ###   ########.fr       */
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

void		WebServ::_create_workers() {
	int 	pipe_fd[2];

	FD_ZERO(&_set_of_pipes);
	for (int i = 0; i < _number_workers; ++i) {
		pipe(pipe_fd);
		if (fork() == 0) {
			close(pipe_fd[STDOUT_FILENO]);
			_worker(pipe_fd[STDIN_FILENO]);
		}
		else {
			close(pipe_fd[STDIN_FILENO]);
			FD_SET(pipe_fd[STDOUT_FILENO], &_set_of_pipes);
			_worker_queue.push(std::make_pair(pipe_fd[STDOUT_FILENO], 0));
		}
	}
}

void		WebServ::run_server() {
	int					 	client_socket;
	std::pair<int, int>		worker;

	while (true) {
		select(_vs_sockets.back() + 1, &_set_of_vs_sockets, nullptr, nullptr, nullptr);
		_get_accept_from_ready_sockets();
	}
}
void		WebServ::_get_accept_from_ready_sockets() {
	int						client_socket;
	std::pair<int, int>		worker;

	for (int socket : _vs_sockets) {
		if (FD_ISSET(socket, &_set_of_vs_sockets)) {
			while ((client_socket = accept(socket, nullptr, nullptr)) != -1) {
				worker = _pop_worker();
				write(worker.first, (char*)&client_socket, 4);
				worker.second += 4;
				if (worker.second >= PIPE_BUFFER_SIZE)
					_pointer_file_to_start(worker.first, worker.second);
				_worker_queue.push(worker);
			}
		}
	}
}

void		WebServ::_worker(int pipe_fd) {
	int		file_position;
	int 	client_socket;

	file_position = 0;
	while (true) {
		read(pipe_fd, (char*)&client_socket, 4);
		file_position += 4;
		if (file_position >= PIPE_BUFFER_SIZE)
			_pointer_file_to_start(pipe_fd, file_position);
		_serve_client(client_socket);
		close(client_socket);
	}
}

void		WebServ::_give_response() {

}

void		WebServ::_serve_client(int client_socket) {
		Request			request;
		Location		location;
	try {
		request = _get_request(client_socket);
		request.set_virtual_server(_list_virtual_servers);
		request.set_location();
		location = request.get_location();
		switch (location.get_location_type()) {
			case _default:
				_default_handler(request, client_socket);
			case cgi:
				_cgi_handler(request, client_socket);
			case proxy:
				_proxy_handler(request, client_socket);
		}
	}
	catch (Request301Redirect& redirect_301) {
		redirect_301.send_response(client_socket);
	}
	catch (RequestException& request_error) {
		request_error.send_response(client_socket);
	}
}

Request		WebServ::_get_request(int client_socket) {
	return (Request());
}

void		WebServ::_default_handler(Request& request, int client_socket) {
	const Location&		location = request.get_location();
	const std::string&	path_to_file = location.get_root() + request.get_target().substr(location.get_path().length());
	struct stat			buff;

	if (stat(path_to_file.c_str(), &buff) == -1)
		throw RequestException("404", "Not Found", request.get_virtual_server().get_error_page("404"));
	else if (S_ISDIR(buff.st_mode) && path_to_file.back() != '/')
		throw Request301Redirect("http://" + request.get_host() + ":" + request.get_port() + "/" + request.get_target() + "/");
	else if (request.get_location().is_allow_method(request.get_method()))
		throw RequestException("405", "Method Not Allowed", request.get_virtual_server().get_error_page("405"));

	if (request.get_method() == "POST")
		_post_method_handler(request, &buff);
	else
		_get_head_methods_handler(request, &buff, client_socket);
}

void	WebServ::_get_head_methods_handler(Request& request, struct stat* buff, int client_socket) {
	const Location&		location = request.get_location();
	const std::string&	path_to_file = location.get_root() + request.get_target().substr(location.get_path().length());

	if (S_ISREG(buff->st_mode) || S_ISLNK(buff->st_mode)) {
		_static_file_send(request, path_to_file, client_socket);
	}
	else if (S_ISDIR(buff->st_mode) && location.is_autoindex())
		"send autoindex page";
}

void WebServ::_static_file_send(Request& request, const std::string& path_to_file, int client_socket) {
	std::ifstream		file(path_to_file);
	std::string			body_response;
	std::string			response;
	char				date[80];

	getline(file, body_response, '\0');
	response = "HTTP/1.1 200 OK\r\n"
			"Server: WebServ/0.1\r\n"
   			"Date: " + std::string(date) + "\r\n"
			"Content-Length: " + std::to_string(body_response.length()) + "\r\n"
			"Last-Modified: " + "\r\n"
			"\r\n" +
			body_response;
	send(client_socket, response.c_str(), response.length(), 0);
}

void WebServ::_post_method_handler(Request& request, struct stat* buff) {
	if (S_ISDIR(buff->st_mode))
		throw RequestException("403", "Forbidden", request.get_virtual_server().get_error_page("403"));
	else
		throw RequestException("405", "Method Not Allowed", request.get_virtual_server().get_error_page("405"));
}

void					WebServ::_pointer_file_to_start(int& fd, int& file_position) {
	file_position = 0;
	lseek(fd, 0, SEEK_SET);
}

std::pair<int, int>		WebServ::_pop_worker() {
	std::pair<int, int>		worker = _worker_queue.front();

	_worker_queue.pop();
	return (worker);
}

VirtualServer		WebServ::_get_virtual_server(const Request& request) const {
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
					if (request.get_header(HOST) == server_name)
						return (server);
			}
		}
	}
	return (*default_vs);
}

