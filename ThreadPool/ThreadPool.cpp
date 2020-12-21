/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 18:37:33 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 20:56:09 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThreadPool.hpp"

ThreadPool::ThreadPool() {
	if ((queue_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == 0)
		throw std::exception();
	pthread_mutex_init(queue_mutex, 0);

	if ((read_stage_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == 0)
		throw std::exception();
	pthread_mutex_init(read_stage_mutex, 0);
	if ((parse_stage_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == 0)
		throw std::exception();
	pthread_mutex_init(parse_stage_mutex, 0);
	if ((generate_stage_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == 0)
		throw std::exception();
	pthread_mutex_init(generate_stage_mutex, 0);
	if ((send_stage_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == 0)
		throw std::exception();
	pthread_mutex_init(send_stage_mutex, 0);
}

ThreadPool::~ThreadPool() {
	pthread_mutex_destroy(queue_mutex);
	free(queue_mutex);
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
