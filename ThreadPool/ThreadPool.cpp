/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 18:37:33 by imicah            #+#    #+#             */
/*   Updated: 2020/12/12 05:32:56 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThreadPool.hpp"

ThreadPool::ThreadPool() {
	if ((_queue_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == nullptr)
		throw std::exception();
	pthread_mutex_init(_queue_mutex, 0);
}

ThreadPool::~ThreadPool() {
	pthread_mutex_destroy(_queue_mutex);
	free(_queue_mutex);
}

Client*		ThreadPool::PopTask() {
	Client*		client = _tasks_queue.front();

	_tasks_queue.pop();
	return (client);
}

void				ThreadPool::PushTask(Client* client) {
	pthread_mutex_lock(_queue_mutex);
	client->SetProcessed(true);
	_tasks_queue.push(client);
	pthread_mutex_unlock(_queue_mutex);
}

bool				ThreadPool::QueueIsEmpty() const { return (_tasks_queue.empty()); }

void				ThreadPool::LockQueueMutex() { pthread_mutex_lock(_queue_mutex); }
void				ThreadPool::UnlockQueueMutex() { pthread_mutex_unlock(_queue_mutex); }
