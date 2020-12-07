/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 18:37:33 by imicah            #+#    #+#             */
/*   Updated: 2020/12/07 01:13:05 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThreadPool.hpp"

ThreadPool::ThreadPool() {
	if ((_read_write_is_queue_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t))) == nullptr)
		throw std::exception();
	pthread_mutex_init(_read_write_is_queue_mutex, nullptr);
}

ThreadPool::~ThreadPool() {
	pthread_mutex_destroy(_read_write_is_queue_mutex);
	free(_read_write_is_queue_mutex);
}

HttpObject*		ThreadPool::pop_task() {
	HttpObject*		http_object = _tasks_queue.front();

	_tasks_queue.pop();
	return (http_object);
}

pthread_mutex_t*	ThreadPool::get_read_write_in_queue_mutex() const { return (_read_write_is_queue_mutex); }

void				ThreadPool::push_task(HttpObject* http_object) { _tasks_queue.push(http_object); }

bool				ThreadPool::queue_is_empty() const { return (_tasks_queue.empty()); }

