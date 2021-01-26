/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_workers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 14:46:29 by imicah            #+#    #+#             */
/*   Updated: 2021/01/26 15:15:26 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
				case parsing_request:
					thread_pool.lockReadStageMutex();
					web_serv.readRequest(client);
					thread_pool.unlockReadStageMutex();
					break;
				case generate_response:
					thread_pool.lockGenerateStageMutex();
					web_serv.generateResponse(client);
					thread_pool.unlockGenerateStageMutex();
					break;
				case send_response:
					thread_pool.lockSendStageMutex();
					web_serv.sendResponse(client);
					thread_pool.unlockSendStageMutex();
					break;
			}
			if (client->getStage() != parsing_request && client->getStage() != close_connection)
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

void	WebServ::createWorkers() {
	pthread_t		worker_thread;

	workers.reserve(number_workers);
	for (int i = 0; i < number_workers; ++i) {
		pthread_create(&worker_thread, 0, worker, (void*)this);
		workers.push_back(worker_thread);
	}
}
