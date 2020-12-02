/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer_workers.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 14:46:29 by imicah            #+#    #+#             */
/*   Updated: 2020/12/02 14:46:29 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

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

void					WebServ::_pointer_file_to_start(int& fd, int& file_position) {
	file_position = 0;
	lseek(fd, 0, SEEK_SET);
}

std::pair<int, int>		WebServ::_pop_worker() {
	std::pair<int, int>		worker = _worker_queue.front();

	_worker_queue.pop();
	return (worker);
}
