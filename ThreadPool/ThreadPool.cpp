/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 18:37:33 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 14:59:24 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThreadPool.hpp"

ThreadPool::ThreadPool() {
	queue_mutex = new pthread_mutex_t;
	pthread_mutex_init(queue_mutex, 0);

	read_stage_mutex = new pthread_mutex_t;
	pthread_mutex_init(read_stage_mutex, 0);

	parse_stage_mutex = new pthread_mutex_t;
	pthread_mutex_init(parse_stage_mutex, 0);

	generate_stage_mutex = new pthread_mutex_t;
	pthread_mutex_init(generate_stage_mutex, 0);

	send_stage_mutex = new pthread_mutex_t;
	pthread_mutex_init(send_stage_mutex, 0);
}

ThreadPool::~ThreadPool() {
	pthread_mutex_destroy(queue_mutex);
	delete queue_mutex;

	pthread_mutex_destroy(read_stage_mutex);
	delete queue_mutex;

	pthread_mutex_destroy(parse_stage_mutex);
	delete queue_mutex;

	pthread_mutex_destroy(generate_stage_mutex);
	delete queue_mutex;

	pthread_mutex_destroy(send_stage_mutex);
	delete queue_mutex;
}

Client*		ThreadPool::popTask() {
	Client*		client = tasks_queue.front();

	tasks_queue.pop();
	return (client);
}

void				ThreadPool::pushTask(Client* client) {
	pthread_mutex_lock(queue_mutex);
	client->setProcessed(true);
	tasks_queue.push(client);
	pthread_mutex_unlock(queue_mutex);
}

bool				ThreadPool::queueIsEmpty() const { return (tasks_queue.empty()); }

void				ThreadPool::lockQueueMutex() { pthread_mutex_lock(queue_mutex); }
void				ThreadPool::unlockQueueMutex() { pthread_mutex_unlock(queue_mutex); }

void				ThreadPool::lockReadStageMutex() { pthread_mutex_lock(read_stage_mutex); }
void				ThreadPool::unlockReadStageMutex() { pthread_mutex_unlock(read_stage_mutex); }

void				ThreadPool::lockParseStageMutex() { pthread_mutex_lock(parse_stage_mutex); }
void				ThreadPool::unlockParseStageMutex() { pthread_mutex_unlock(parse_stage_mutex); }

void				ThreadPool::lockGenerateStageMutex() { pthread_mutex_lock(generate_stage_mutex); }
void				ThreadPool::unlockGenerateStageMutex() { pthread_mutex_unlock(generate_stage_mutex); }

void				ThreadPool::lockSendStageMutex() { pthread_mutex_lock(send_stage_mutex); }
void				ThreadPool::unlockSendStageMutex() { pthread_mutex_unlock(send_stage_mutex); }
