/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 18:34:43 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 23:35:08 by imicah           ###   ########.fr       */
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
	pthread_mutex_t*		parsing_stage_mutex;
	pthread_mutex_t*		generate_stage_mutex;
	pthread_mutex_t*		send_stage_mutex;

	ThreadPool();
	~ThreadPool();

	inline bool				queueIsEmpty() const { return (tasks_queue.empty()); }

	inline void				lockQueueMutex() const { pthread_mutex_lock(queue_mutex); }
	inline void				unlockQueueMutex() const { pthread_mutex_unlock(queue_mutex); }

	inline void				lockReadStageMutex() const { pthread_mutex_lock(parsing_stage_mutex); }
	inline void				unlockReadStageMutex() const { pthread_mutex_unlock(parsing_stage_mutex); }
	inline void				lockGenerateStageMutex() const { pthread_mutex_lock(generate_stage_mutex); }
	inline void				unlockGenerateStageMutex() const { pthread_mutex_unlock(generate_stage_mutex); }
	inline void				lockSendStageMutex() const { pthread_mutex_lock(send_stage_mutex); }
	inline void				unlockSendStageMutex() const { pthread_mutex_unlock(send_stage_mutex); }

	void		 			pushTask(Client* client);
	Client*					popTask();
};

#endif //WEBSERV_THREADPOOL_HPP
