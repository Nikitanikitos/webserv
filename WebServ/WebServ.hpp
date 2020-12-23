/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:07 by nikita            #+#    #+#             */
/*   Updated: 2020/12/22 19:41:02 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_WEBSERV_HPP
# define WEBSERV_WEBSERV_HPP

# include <vector>
# include <sys/stat.h>
# include <dirent.h>

# include "libft.hpp"
# include "Client.hpp"
# include "VirtualServer.hpp"
# include "ThreadPool.hpp"
# include "HandlerHttpObject.hpp"

class	WebServ {
private:
	friend void*	worker(void*);

	std::vector<Client*>			clients;
	std::vector<VirtualServer*>		virtual_servers;

	int 							number_workers;
	ThreadPool						thread_pool;

	static void					getHeadMethodHandler(Client* client, Location* location, VirtualServer* virtual_server,
																			t_stat* info, std::string& path_to_target);
	static void					putMethodHandler(Client* client, Location* location, VirtualServer* virtual_server,
																			t_stat* info, std::string& path_to_target);

	static bytes				autoindexGenerate(HttpRequest *request, const std::string& path_to_target);

	void						createWorkers();

	VirtualServer*				getVirtualServer(Client *client) const;

	void						readRequest(Client* client);
	void						generateResponse(Client* client);
	void						sendResponse(Client* client);

	static std::string			getPathToTarget(HttpRequest *request, Location* location);

	void						addNewClient(fd_set& readfd_set);
	void						addClientSocketInSet(fd_set& readfd_set, fd_set& writefd_set, int& max_fd);
	void						addClientInTaskQueue(fd_set& readfd_set, fd_set& writefd_set);
	void						initSets(fd_set &writefd_set, fd_set &readfd_set, int &max_fd);

	static void					setErrorPage(Client *client, Location *location, VirtualServer *virtual_server);
	static bytes				generateErrorPage(const std::string& code);

public:
	static int working;

	explicit WebServ(int number_of_workers);
	virtual ~WebServ();

	void						runServer();
	void						addVirtualServer(VirtualServer *virtual_server);
};

#endif //WEBSERV_WEBSERV_HPP
