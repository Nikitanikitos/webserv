/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_workers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 14:46:29 by imicah            #+#    #+#             */
/*   Updated: 2020/12/09 07:51:14 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "WebServ.hpp"

[[noreturn]] void*	worker(void* arg) {
	WebServ&		web_serv = *(WebServ*)arg;
	ThreadPool&		thread_pool = web_serv._thread_pool;

	while (true) {
		pthread_mutex_lock(thread_pool.get_read_write_in_queue_mutex());
		if (!thread_pool.queue_is_empty()) {
			Client*		client = thread_pool.pop_task();
			pthread_mutex_unlock(thread_pool.get_read_write_in_queue_mutex());
			switch (client->get_stage()) {
				case read_request_:
					web_serv._read_request(client);
				case parsing_request_:
					web_serv._parsing_request(client);
				case generate_response_:
					web_serv._generate_response(client);
				case send_response_:
					web_serv._send_response(client);
				case close_connection_:
					web_serv._close_connection(client);
			}
			if (client->get_stage() != read_request_ && client->get_stage() != send_response_)
				thread_pool.push_task(client);
		}
		else
			pthread_mutex_unlock(thread_pool.get_read_write_in_queue_mutex());
		usleep(500);
	}
}

void		WebServ::_create_workers() {
	pthread_t		worker_thread;

	for (int i = 0; i < _number_workers; ++i) {
		pthread_create(&worker_thread, nullptr, worker, (void*)this);
		pthread_detach(worker_thread);
	}
}

void WebServ::set_number_workers(int number_workers) { _number_workers = number_workers; }
