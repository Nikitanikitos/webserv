/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 18:34:43 by imicah            #+#    #+#             */
/*   Updated: 2020/12/05 18:34:43 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_THREADPOOL_HPP
# define WEBSERV_THREADPOOL_HPP

# include <queue>
# include "HttpObject.hpp"

class ThreadPool {
private:
	std::queue<HttpObject>		_tasks_queue;
	pthread_mutex_t*			_mutex_check_tasks_queue;

public:
	ThreadPool();
	~ThreadPool();

	[[nodiscard]] pthread_mutex_t*		get_mutex_check_tasks_queue() const;
	[[nodiscard]] bool					queue_is_empty() const;

	HttpObject&							pop_task();
	void 								push_task(const HttpObject&);

};


#endif //WEBSERV_THREADPOOL_HPP
