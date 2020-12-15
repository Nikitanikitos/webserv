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
	pthread_mutex_init(_queue_mutex, NULL);
}

ThreadPool::~ThreadPool() {
	pthread_mutex_destroy(_queue_mutex);
	free(_queue_mutex);
}

Client*		ThreadPool::pop_task() {
	Client*		client = _tasks_queue.front();

	_tasks_queue.pop();
	return (client);
}

pthread_mutex_t*	ThreadPool::get_read_write_in_queue_mutex() const { return (_queue_mutex); }

void				ThreadPool::push_task(Client* client) {
	pthread_mutex_lock(_queue_mutex);
	client->set_processed(true);
	_tasks_queue.push(client);
	pthread_mutex_unlock(_queue_mutex);
}

bool				ThreadPool::queue_is_empty() const { return (_tasks_queue.empty()); }

void ThreadPool::lock_queue_mutex() {
	pthread_mutex_lock(_queue_mutex);
}

void ThreadPool::unlock_queue_mutex() {
	pthread_mutex_unlock(_queue_mutex);
}
