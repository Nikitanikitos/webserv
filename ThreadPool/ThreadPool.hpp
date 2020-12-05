//
// Created by Irving Micah on 12/5/20.
//

#ifndef WEBSERV_THREADPOOL_HPP
#define WEBSERV_THREADPOOL_HPP

# include <queue>

class ThreadPool {
private:
	std::queue<HttpObject>		_tasks_queue;

};


#endif //WEBSERV_THREADPOOL_HPP
