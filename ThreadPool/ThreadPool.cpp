/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 18:37:33 by imicah            #+#    #+#             */
/*   Updated: 2020/12/05 18:37:33 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThreadPool.hpp"

ThreadPool::ThreadPool() {
	if ((_mutex_check_tasks_queue = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == nullptr)
		throw std::exception();
	pthread_mutex_init(_mutex_check_tasks_queue, nullptr); }

ThreadPool::~ThreadPool() {
	pthread_mutex_destroy(_mutex_check_tasks_queue);
	free(_mutex_check_tasks_queue);
}

HttpObject ThreadPool::pop_task() {
	HttpObject		http_object = _tasks_queue.front();

	_tasks_queue.pop();
	return (http_object);
}

pthread_mutex_t*	ThreadPool::get_mutex_check_tasks_queue() const { return (_mutex_check_tasks_queue); }

void ThreadPool::push_task(const HttpObject& http_object) { _tasks_queue.push(http_object); }

bool ThreadPool::queue_is_empty() const {
	return (_tasks_queue.empty());
}
