/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 18:34:43 by imicah            #+#    #+#             */
/*   Updated: 2020/12/07 01:13:05 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_THREADPOOL_HPP
# define WEBSERV_THREADPOOL_HPP

# include <queue>
# include "HttpObject.hpp"
# include "pthread.h"

class ThreadPool {
private:
	std::queue<HttpObject*>		_tasks_queue;
	pthread_mutex_t*			_read_write_is_queue_mutex;

public:
	ThreadPool();
	~ThreadPool();

	[[nodiscard]] pthread_mutex_t*		get_read_write_in_queue_mutex() const;
	[[nodiscard]] bool					queue_is_empty() const;

	HttpObject*							pop_task();
	void 								push_task(HttpObject*);

};


#endif //WEBSERV_THREADPOOL_HPP
