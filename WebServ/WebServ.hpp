/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:07 by nikita            #+#    #+#             */
/*   Updated: 2020/12/21 12:42:53 by imicah           ###   ########.fr       */
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

class WebServ {
private:
	friend void*	worker(void*);

	static std::string				methods[6];

	std::vector<Client*>			clients;
	std::vector<VirtualServer*>		virtual_servers;

	int 							number_workers;
	ThreadPool						thread_pool;

	static void					getHeadMethodHandler(Client* client, Location* location, VirtualServer* virtual_server, struct stat* buff,
														std::string& path_to_target);
	static void					putMethodHandler(Client* client, Location* location, VirtualServer* virtual_server,
													struct stat* buff, std::string& path_to_target);
	void						cgiHandler(const HttpRequest&, const VirtualServer&, const Location&, int);

	static bytes				autoindexGenerate(HttpRequest *request, const std::string& path_to_target);

	void						createWorkers();

	VirtualServer*				getVirtualServer(Client *client) const;

	void						readRequest(Client* client);
	void						parsingRequest(Client* client);
	void						generateResponse(Client* client);
	void						sendResponse(Client* client);

	static std::string			getPathToTarget(HttpRequest *request, Location* location);
	std::vector<std::string>	getArgs(const std::string& line, char separate) const;
	std::vector<std::string>	trimRequest(std::string const& buff, HttpRequest* request) const;
	bool						checkCountSpace(const std::string& line, int numSpaces) const;
	bool						checkMethod(std::string method, int size) const;
	void						strToLower(std::string& str) const;
	void						setBadRequestResponse(Client* client);

	void						addNewClient(fd_set& readfd_set);
	void						addClientSocketInSet(fd_set& readfd_set, fd_set& writefd_set, int& max_fd);
	void						addClientInTaskQueue(fd_set& readfd_set, fd_set& writefd_set);
	void						initSets(fd_set &writefd_set, fd_set &readfd_set, int &max_fd);
	std::vector<std::string>	getKeyValue(const std::string &line) const;

	static void					setErrorPage(Client *client, Location *location, VirtualServer *virtual_server);
	static bytes				generateErrorPage(const std::string& code);
	static bool					isErrorStatus(const std::string& status);

public:
	static int working;

	explicit WebServ(int number_of_workers);
	virtual ~WebServ();

	void						runServer();
	void						addVirtualServer(VirtualServer *virtual_server);
};

#endif //WEBSERV_WEBSERV_HPP
