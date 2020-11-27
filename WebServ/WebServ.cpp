/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/11/27 17:26:22 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	}
}

void		WebServ::_parse_request_body() {

}

void		WebServ::_give_response() {

}

void		WebServ::_serve_client(int client_socket) {
	try {
		Request			request = _get_request(client_socket);
		VirtualServer	server = _get_virtual_server(request);
		Location		location = server._get_location(request);

		switch (location.get_location_type()) {
			case _default:
				_default_handler(request, client_socket, location);
			case cgi:
				_cgi_handler(request, client_socket, location);
			case proxy:
				_proxy_handler(request, client_socket, location);
		}
	}
	catch (Request400Error& request_400_error) {
		request_400_error.send_response(client_socket);
	}
	catch (Request405Error& request_405_error) {
		request_405_error.send_response(client_socket);
	}
	catch (Request505Error& request_505_error) {
		request_505_error.send_response(client_socket);
	}
	catch (Request404Error& request_404_error) {
		request_404_error.send_response(client_socket);
	}
}

Request		WebServ::_get_request(int client_socket) {
	return (Request());
}

void	WebServ::_default_handler(Request& request, int client_socket, Location& location) {
	Response	response;

	if (!_is_allow_method(request.get_method(), location.get_allow_methods()))
		response.set_status_code("405");
	else if (!_is_file_found(request.get_target(), location.get_root()))
		response.set_status_code("404");
}

bool	WebServ::_is_allow_method(const std::string& method, const std::vector<bool>& allow_methods) {
	static std::string	methods[NUMBER_METHODS] = {"GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS"};

	for (int i = 0; i < NUMBER_METHODS; ++i) {
		if (method == methods[i])
			return (allow_methods[i]);
	}
	return (false);
}

bool	WebServ::_is_file_found(const std::string& target, const std::string& root) {
	return (false);
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
