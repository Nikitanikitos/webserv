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
# include <cstring>
# include <wait.h>

# include "libft.hpp"
# include "Client.hpp"
# include "VirtualServer.hpp"
# include "ThreadPool.hpp"

enum { count_error_pages = 7, };

class	WebServ {
private:
	friend void*	worker(void*);

	std::vector<Client*>			clients;
	std::vector<VirtualServer*>		virtual_servers;

	int 							number_workers;
	ThreadPool						thread_pool;

	static void					DefaultHandler(Client* client, Location* location, VirtualServer* virtual_server,
																		t_stat* info, std::string& path_to_target);
	static void					putMethodHandler(Client* client, Location* location, VirtualServer* virtual_server,
																			t_stat* info, std::string& path_to_target);
	void cgiHandler(Client *client, const std::string &path_to_target, Location *location);

	static bytes				autoindexGenerate(HttpRequest *request, const std::string& path_to_target);

	void						createWorkers();

	VirtualServer*				getVirtualServer(Client *client) const;

	void						readRequest(Client* client);
	void						generateResponse(Client* client);
	void						sendResponse(Client* client);

	static std::string			getPathToTarget(HttpRequest *request, Location* location);

	void						addNewClient(fd_set& readfd_set);
	void						deleteClient(std::vector<Client*>::iterator& client);
	void						addClientSocketInSet(fd_set& readfd_set, fd_set& writefd_set, int& max_fd);
	void						addClientInTaskQueue(fd_set& readfd_set, fd_set& writefd_set);
	void						initSets(fd_set &writefd_set, fd_set &readfd_set, int &max_fd);

	static void					setErrorPage(Client *client, Location *location, VirtualServer *virtual_server);
	static bytes				generateErrorPage(const std::string& code);

	static bool					checkAuth(Client* client, const std::string& root);
	static bool					checkValidAuth(const std::string& login_password, const std::string& path_to_htpasswd);
	static void					getInfoOutHtaccess(int fd, std::string& realm, std::string& path_to_htpasswd);

	std::string isErrorRequest(Location* location, t_stat& info, Client* client);
	void 						setEnvForCgi(char **env, Client *client, const std::string &path_to_target);

public:
	static int		working;

	explicit WebServ(int number_of_workers) : number_workers(number_of_workers) { }
	virtual ~WebServ() {
		for (int i = 0; i < clients.size(); ++i)
			delete clients[i];
	};

	void						runServer();
	void						addVirtualServer(VirtualServer *virtual_server);
};

#endif //WEBSERV_WEBSERV_HPP
