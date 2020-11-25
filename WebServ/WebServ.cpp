/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:48:56 by nikita            #+#    #+#             */
/*   Updated: 2020/11/25 04:54:54 by imicah           ###   ########.fr       */
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

std::map<std::string, std::string>		WebServ::_check_request_header() {
	return (std::map<std::string, std::string>());
}

void		WebServ::_execute_cgi_client() {

}

void		WebServ::_parse_request_body() {

}

void		WebServ::_give_response() {

}

void		WebServ::_serve_client(int client_socket) {
	typedef		void (WebServ::*handler_t)(Request&);

	handler_t	handlers[3] = {&WebServ::_default_handler, &WebServ::_cgi_handler, &WebServ::_proxy_handler};
	Request		request = _get_request(client_socket);
	Location	location = _get_location(request);

	for (int i = 0; i < 2; ++i) {
		if (location.location_type == i)
			(this->*handlers[i])(request);
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

void		WebServ::_worker(int pipe_fd) {
	fd_set	pipe_set;
	int		file_position;
	int 	client_socket;

	file_position = 0;
	FD_ZERO(&pipe_set);
	FD_SET(pipe_fd, &pipe_set);
	while (true) {
		select(pipe_fd + 1, &pipe_set, nullptr, nullptr, nullptr);
		read(pipe_fd, (char*)&client_socket, 4);
		file_position += 4;
		if (file_position >= PIPE_BUFFER_SIZE)
			_pointer_file_to_start(pipe_fd, file_position);
		_serve_client(client_socket);
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

void		WebServ::_pointer_file_to_start(int& fd, int& file_position) {
	file_position = 0;
	lseek(fd, 0, SEEK_SET);
}

Request		WebServ::_get_request(int client_socket) {
	return (Request());
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

std::pair<int, int>		WebServ::_pop_worker() {
	std::pair<int, int>		worker = _worker_queue.front();

	_worker_queue.pop();
	return (worker);
}
