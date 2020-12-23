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

	parsing_stage_mutex = new pthread_mutex_t;
	pthread_mutex_init(parsing_stage_mutex, 0);

	generate_stage_mutex = new pthread_mutex_t;
	pthread_mutex_init(generate_stage_mutex, 0);

	send_stage_mutex = new pthread_mutex_t;
	pthread_mutex_init(send_stage_mutex, 0);
}

ThreadPool::~ThreadPool() {
	pthread_mutex_destroy(queue_mutex);
	delete queue_mutex;

	pthread_mutex_destroy(parsing_stage_mutex);
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
