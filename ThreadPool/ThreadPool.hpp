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
# include "Client.hpp"
# include "pthread.h"

class ThreadPool {
private:
	std::queue<Client*>		_tasks_queue;
	pthread_mutex_t*		_queue_mutex;

public:
	ThreadPool();
	~ThreadPool();

	bool					QueueIsEmpty() const;

	void					LockQueueMutex();
	void					UnlockQueueMutex();
	void 					PushTask(Client* client);
	Client*					PopTask();
};

#endif //WEBSERV_THREADPOOL_HPP
