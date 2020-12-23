/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_workers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 14:46:29 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 23:44:40 by imicah           ###   ########.fr       */
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
				case parsing_request_:
					thread_pool.lockReadStageMutex();
					web_serv.readRequest(client);
					thread_pool.unlockReadStageMutex();
					break;
				case generate_response_:
					thread_pool.lockGenerateStageMutex();
					web_serv.generateResponse(client);
					thread_pool.unlockGenerateStageMutex();
					break;
				case send_response_:
					thread_pool.lockReadStageMutex();
					web_serv.sendResponse(client);
					thread_pool.unlockReadStageMutex();
					break;
			}
			if (client->getStage() != parsing_request_ && client->getStage() != close_connection_ && client->getStage() != parsing_request_)
				thread_pool.pushTask(client);
			else
				client->setProcessed(false);
		}
		else
			thread_pool.unlockQueueMutex();
		usleep(1000);
	}
	return (0);
}

void		WebServ::createWorkers() {
	pthread_t		worker_thread;

	for (int i = 0; i < number_workers; ++i) {
		pthread_create(&worker_thread, 0, worker, (void*)this);
		pthread_detach(worker_thread);
	}
}
