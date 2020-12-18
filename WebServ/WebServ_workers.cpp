/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_workers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 14:46:29 by imicah            #+#    #+#             */
/*   Updated: 2020/12/18 04:03:52 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "WebServ.hpp"

void*	worker(void* arg) {
	WebServ&		web_serv = *(WebServ*)arg;
	ThreadPool&		thread_pool = web_serv._thread_pool;

	while (true) {
		thread_pool.LockQueueMutex();
		if (!thread_pool.QueueIsEmpty()) {
			Client*		client = thread_pool.PopTask();
			thread_pool.UnlockQueueMutex();
			switch (client->GetStage()) {
				case read_request_:
					web_serv.ReadRequest(client);
					break;
				case parsing_request_:
					web_serv.ParsingRequest(client);
					break;
				case generate_response_:
					web_serv.GenerateResponse(client);
					break;
				case send_response_:
					web_serv.SendResponse(client);
					break;
			}
			if (client->GetStage() != read_request_ && client->GetStage() != close_connection_ && client->GetStage() != read_request_)
				thread_pool.PushTask(client);
			else
				client->SetProcessed(false);
		}
		else
			thread_pool.UnlockQueueMutex();
		usleep(500);
	}
}

void		WebServ::_CreateWorkers() {
	pthread_t		worker_thread;

	for (int i = 0; i < _number_workers; ++i) {
		pthread_create(&worker_thread, nullptr, worker, (void*)this);
		pthread_detach(worker_thread);
	}
}
