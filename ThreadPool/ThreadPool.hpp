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
	pthread_mutex_t*		read_stage_mutex;
	pthread_mutex_t*		parse_stage_mutex;
	pthread_mutex_t*		generate_stage_mutex;
	pthread_mutex_t*		send_stage_mutex;

	ThreadPool();
	~ThreadPool();

	bool					queueIsEmpty() const;

	void					lockQueueMutex();
	void					unlockQueueMutex();

	void					lockReadStageMutex();
	void					unlockReadStageMutex();

	void					lockParseStageMutex();
	void					unlockParseStageMutex();
	void					lockGenerateStageMutex();
	void					unlockGenerateStageMutex();
	void					lockSendStageMutex();
	void					unlockSendStageMutex();

	void 					pushTask(Client* client);
	Client*					popTask();
};

#endif //WEBSERV_THREADPOOL_HPP
