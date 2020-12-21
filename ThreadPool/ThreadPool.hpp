/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 18:34:43 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:40:42 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_THREADPOOL_HPP
# define WEBSERV_THREADPOOL_HPP

# include <queue>
# include "Client.hpp"
# include <pthread.h>

class ThreadPool {
private:
	std::queue<Client*>		tasks_queue;
	pthread_mutex_t*		queue_mutex;

public:
	ThreadPool();
	~ThreadPool();

	bool					queueIsEmpty() const;

	void					lockQueueMutex();
	void					unlockQueueMutex();
	void 					pushTask(Client* client);
	Client*					popTask();
};

#endif //WEBSERV_THREADPOOL_HPP
