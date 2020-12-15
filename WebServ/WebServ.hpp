/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:07 by nikita            #+#    #+#             */
/*   Updated: 2020/12/12 08:17:18 by imicah           ###   ########.fr       */
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

	std::vector<int>						_sockets;
	std::vector<Client*>					_clients;
	std::vector<VirtualServer>				_list_virtual_servers;

	int 									_number_workers;
	ThreadPool								_thread_pool;

	static void			_default_handler(Client *http_object, const VirtualServer& virtual_server, const Location& location);
	void				_cgi_handler(const Request&, const VirtualServer&, const Location&, int);
	void				_proxy_handler(const Request&, const VirtualServer&, const Location&, int);

	static void			_POST_method_handler(const Request& request, struct stat* buff, const VirtualServer& virtual_server);
	static void			_GET_HEAD_methods_handler(Client *http_object, struct stat* buff, const Location& location);

	static Response*	_static_file_handler(const Request& request, const std::string& path_to_file);
	static Response*	_autoindex_handler(const Request& request, const std::string& path_to_target);

	static std::string	_autoindex_generate(const Request& request, const std::string& path_to_target);

	void				_create_workers();

	const VirtualServer&	_get_virtual_server(Request *request) const;

	void		read_request(Client*);
	void		parsing_request(Client *client);
	void		generate_response(Client *client);
	void		send_response(Client *http_object);
	void		close_connection(Client*);

	static std::string		_get_path_to_target(const Request&, const Location&);
	std::vector<std::string> _getArgs(std::string const& line) const;
	std::vector<std::string> _trimRequest(std::string const& buff) const;
	bool _checkCountSpace(std::string const& line, int numSpaces) const;
	bool _checkMethod(std::string method, int size) const;
	void 	_strToLower(std::string& str) const;


public:
	explicit WebServ(const std::vector<VirtualServer> &list_virtual_servers, int number_of_workers);
	/* Инициализируем _set_vs_sockets, добавляем все сокеты виртуальных серверов в _set_vs_sockets, */

	virtual ~WebServ();

	void	run_server();
	/* Метод запускает сервер. Вызывает select() на серверные сокеты с бесконечным тайм-аутом,
	 * Проверяет в какой сокет поступило соединение и вызывает метод serve_client(), передавая туда сокет клиента */

	void add_new_client(fd_set readfd_set);

	void	set_number_workers(int number_workers);

	void add_client_socket_in_set(fd_set &readfd_set, int &max_fd);

	void add_client_in_task_queue(fd_set &readfd_set);

	void init_sets(fd_set &writefd_set, fd_set &readfd_set, int &max_fd);

	static std::string methods[6];
	static std::string fields[18];
};

#endif //WEBSERV_WEBSERV_HPP
