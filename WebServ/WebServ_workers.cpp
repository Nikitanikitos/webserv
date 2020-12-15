/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_workers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 14:46:29 by imicah            #+#    #+#             */
/*   Updated: 2020/12/12 08:13:37 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "WebServ.hpp"

void*	worker(void* arg) {
	WebServ&		web_serv = *(WebServ*)arg;
	ThreadPool&		thread_pool = web_serv._thread_pool;

	while (true) {
		thread_pool.lock_queue_mutex();
		if (!thread_pool.queue_is_empty()) {
			Client*		client = thread_pool.pop_task();
			thread_pool.unlock_queue_mutex();
			switch (client->get_stage()) {
				case read_request_:
					web_serv.read_request(client);
					break;
				case parsing_request_:
					web_serv.parsing_request(client);
					break;
				case generate_response_:
					web_serv.generate_response(client);
					break;
				case send_response_:
					web_serv.send_response(client);
					break;
				case close_connection_:
					web_serv.close_connection(client);
					break;
			}
			if (client->get_stage() != read_request_ && client->get_stage() != close_connection_)
				thread_pool.push_task(client);
			else
				client->set_processed(false);
		}
		else
			thread_pool.unlock_queue_mutex();
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
