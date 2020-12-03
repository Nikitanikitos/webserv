/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:07 by nikita            #+#    #+#             */
/*   Updated: 2020/12/04 00:38:09 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_WEBSERV_HPP
# define WEBSERV_WEBSERV_HPP

# include <string>
# include <map>
# include <queue>
# include <zconf.h>
# include "Request.hpp"
# include "VirtualServer.hpp"
# include "Response.hpp"
# include "exceptions.hpp"
# include <fcntl.h>

# define PIPE_BUFFER_SIZE	64000

class WebServ {
private:
	fd_set									_set_of_vs_sockets;
	fd_set									_set_of_pipes;
	std::vector<int>						_vs_sockets;
	std::vector<VirtualServer>				_list_virtual_servers;
	std::queue<std::pair<int, int> >		_worker_queue;
	int 									_number_workers;

	static void			_default_handler(const Request& request, const VirtualServer& virtual_server, const Location& location, int client_socket);
	void				_cgi_handler(const Request&, const VirtualServer&, const Location&, int);
	void				_proxy_handler(const Request&, const VirtualServer&, const Location&, int);


	static void			_POST_method_handler(const Request& request, struct stat* buff, const VirtualServer& virtual_server);
	static void			_GET_HEAD_methods_handler(const Request& request, struct stat* buff, int client_socket,
													 const Location& location);

	static void			_static_file_handler(const Request& request, const std::string& path_to_file, int client_socket);
	static void			_autoindex_handler(const Request&, const std::string&, int);

	static std::string	_autoindex_generate(const Request& request, const std::string& path_to_target);

	void				_get_accept_from_ready_sockets();
	static void			_pointer_file_to_start(int&, int&);

	void					_create_workers();
	[[noreturn]] void		_worker(int);
	std::pair<int, int>		_pop_worker();

	[[nodiscard]] const VirtualServer&	_get_virtual_server(const Request& request) const;

	[[nodiscard]] Request				_get_request(int);
	/* Метод получает данные запроса через функцию recv().
	 * Из первой строки получает метод, путь и заносит в объект Request.
	 * Далее парсит заголовки, заносит их в словарь и проверяет корректность заголовка через _check_request_header().
	 * Далее парсит тело запроса через _parse_request_body() (если тело запроса есть).
	 * Возвращает объект Request() */

	static std::string		_get_path_to_target(const Request&, const Location&);

	void					_serve_client(int);

public:
	explicit WebServ(const std::vector<VirtualServer>&);
	/* Инициализируем _set_vs_sockets, добавляем все сокеты виртуальных серверов в _set_vs_sockets, */

	virtual ~WebServ();

	[[noreturn]] void	run_server();
	/* Метод запускает сервер. Вызывает select() на серверные сокеты с бесконечным тайм-аутом,
	 * Проверяет в какой сокет поступило соединение и вызывает метод serve_client(), передавая туда сокет клиента */
};

#endif //WEBSERV_WEBSERV_HPP
