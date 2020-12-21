/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_workers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 14:46:29 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:42:53 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "WebServ.hpp"

void*	worker(void* arg) {
	WebServ&		web_serv = *(WebServ*)arg;
	ThreadPool&		thread_pool = web_serv.thread_pool;

	while (WebServ::working) {
		thread_pool.lockQueueMutex();
		if (!thread_pool.queueIsEmpty()) {
			Client*		client = thread_pool.popTask();
			thread_pool.unlockQueueMutex();
			switch (client->getStage()) {
				case read_request_:
					web_serv.readRequest(client);
					break;
				case parsing_request_:
					web_serv.parsingRequest(client);
					break;
				case generate_response_:
					web_serv.generateResponse(client);
					break;
				case send_response_:
					web_serv.sendResponse(client);
					break;
			}
			if (client->getStage() != read_request_ && client->getStage() != close_connection_ &&
					client->getStage() != read_request_)
				thread_pool.pushTask(client);
			else
				client->setProcessed(false);
		}
		else
			thread_pool.unlockQueueMutex();
		usleep(500);
	}
	return (0);
}

void		WebServ::createWorkers() {
	pthread_t		worker_thread;

	for (int i = 0; i < number_workers; ++i) {
		pthread_create(&worker_thread, nullptr, worker, (void*)this);
		pthread_detach(worker_thread);
	}
}
