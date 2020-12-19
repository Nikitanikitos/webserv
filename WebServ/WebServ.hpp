/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:07 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 15:17:51 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_WEBSERV_HPP
# define WEBSERV_WEBSERV_HPP

# include <string>
# include <map>
# include <queue>
# include "VirtualServer.hpp"
# include "Client.hpp"
# include "ResponseException.hpp"
# include "libft.hpp"
# include "ThreadPool.hpp"

class WebServ {
private:
	friend void*	worker(void*);

	static std::string				methods[6];

	std::vector<Client*>			_clients;
	std::vector<VirtualServer*>		_virtual_servers;

	int 							_number_workers;
	ThreadPool						_thread_pool;

	static void					_DefaultHandler(Client *client, Location *location, struct stat& buff, std::string& path_to_target);
	void						_CgiHandler(const Request&, const VirtualServer&, const Location&, int);

	static bytes _AutoindexGenerate(Request *request, const std::string& path_to_target);

	void						_CreateWorkers();

	VirtualServer*				_GetVirtualServer(Client *client) const;

	void						ReadRequest(Client* client);
	void						ParsingRequest(Client* client);
	void						GenerateResponse(Client* client);
	void						SendResponse(Client* client);

	static std::string			_GetPathToTarget(Request *request, Location* location);
	std::vector<std::string>	_GetArgs(const std::string& line, char separate) const;
	std::vector<std::string>	_TrimRequest(const std::string& buff) const;
	bool						_CheckCountSpace(const std::string& line, int numSpaces) const;
	bool						_CheckMethod(std::string method, int size) const;
	void						_StrToLower(std::string& str) const;

	void						_AddNewClient(fd_set& readfd_set);
	void						_AddClientSocketInSet(fd_set& readfd_set, fd_set& writefd_set, int& max_fd);
	void						_AddClientInTaskQueue(fd_set& readfd_set, fd_set& writefd_set);
	void						_InitSets(fd_set &writefd_set, fd_set &readfd_set, int &max_fd);
	std::vector<std::string>	_GetKeyValue(const std::string &line) const;

	bool						_CheckError(Client* client, VirtualServer* virtual_server, Location* location,
																		struct stat& buff, std::string& path_to_target);
	void						_SetErrorPage(Client *client, Location *location, VirtualServer *virtual_server);
	bytes _GenerateErrorPage(const std::string& code) const;

public:
	explicit WebServ(int number_of_workers);
	virtual ~WebServ();

	void	RunServer();
	void	AddVirtualServer(VirtualServer *virtual_server);
};

#endif //WEBSERV_WEBSERV_HPP
