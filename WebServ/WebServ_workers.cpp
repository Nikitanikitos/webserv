/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_workers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 14:46:29 by imicah            #+#    #+#             */
/*   Updated: 2020/12/06 04:31:04 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "WebServ.hpp"

[[noreturn]] void*	worker(void* arg) {
	WebServ&		web_serv = *(WebServ*)arg;
	ThreadPool&		thread_pool = web_serv._thread_pool;

	while (true) {
		pthread_mutex_lock(thread_pool.get_mutex_check_tasks_queue());
		if (!thread_pool.queue_is_empty()) {
			HttpObject		http_object = thread_pool.pop_task();
			pthread_mutex_unlock(thread_pool.get_mutex_check_tasks_queue());
			if (http_object.get_stage() == generate_request_)
				web_serv.generate_request(http_object);
			else if (http_object.get_stage() == generate_response_)
				web_serv.generate_response(http_object);
			else if (http_object.get_stage() == send_response_)
				web_serv.send_response(http_object);
			http_object.next_stage();
			if (http_object.get_stage() != complited_) {
				pthread_mutex_lock(thread_pool.get_mutex_check_tasks_queue());
				thread_pool.push_task(http_object);
				pthread_mutex_unlock(thread_pool.get_mutex_check_tasks_queue());
			}
		}
		else
			pthread_mutex_unlock(thread_pool.get_mutex_check_tasks_queue());
	}
}

void		WebServ::_create_workers() {
	pthread_t		worker_thread;

	for (int i = 0; i < _number_workers; ++i) {
		pthread_create(&worker_thread, nullptr, worker, (void*)this);
		pthread_detach(worker_thread);
	}
}

void		WebServ::_get_accept_from_ready_sockets() {
	int client_socket;

	for (int socket : _vs_sockets) {
		if (FD_ISSET(socket, &_set_of_vs_sockets)) {
			while ((client_socket = accept(socket, nullptr, nullptr)) != -1)
				_thread_pool.push_task(HttpObject(client_socket, generate_request_));
		}
	}
}

void WebServ::set_number_workers(int number_workers) { _number_workers = number_workers; }
