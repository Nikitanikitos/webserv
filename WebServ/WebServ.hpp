/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:07 by nikita            #+#    #+#             */
/*   Updated: 2020/11/27 17:26:22 by imicah           ###   ########.fr       */
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

# define PIPE_BUFFER_SIZE	64000

class WebServ {
private:
	fd_set									_set_of_vs_sockets;
	fd_set									_set_of_pipes;
	std::vector<int>						_vs_sockets;
	std::vector<VirtualServer>				_list_virtual_servers;
	std::queue<std::pair<int, int> >		_worker_queue;
	int 									_number_workers;

	static bool			_is_allow_method(const std::string&, const std::vector<bool>&);
	static bool			_is_file_found(const std::string&, const std::string&);

	void				_default_handler(Request&, int, Location&);
	void 				_cgi_handler(Request&, int, Location&);
	void				_proxy_handler(Request&, int, Location&);

	void					_get_accept_from_ready_sockets();
	static void				_pointer_file_to_start(int&, int&);

	void				_parse_request_body(); // TODO что метод возращает и возвращает ли вообще?
	/* Метод парсит тело запроса */

	void					_create_workers();
	[[noreturn]] void		_worker(int);
	std::pair<int, int>		_pop_worker();

	Request					_get_request(int);
	/* Метод получает данные запроса через функцию recv().
	 * Из первой строки получает метод, путь и заносит в объект Request.
	 * Далее парсит заголовки, заносит их в словарь и проверяет корректность заголовка через _check_request_header().
	 * Далее парсит тело запроса через _parse_request_body() (если тело запроса есть).
	 * Возвращает объект Request() */
	VirtualServer			_get_virtual_server(const Request&) const;

	void					_give_response();
	/* Принимает объект Request(), создает объект Response(), составляет тело ответа и отсылает его через send() */

	void				_serve_client(int);
	/* Метод вызывает функцию accept() на сокет и вызывает метод _get_request(),
	 * который возвращает объект Request.
	 * вызов метода _give_response() */

public:
	explicit WebServ(const std::vector<VirtualServer>&);
	/* Инициализируем _set_vs_sockets, добавляем все сокеты виртуальных серверов в _set_vs_sockets, */

	virtual ~WebServ();

	[[noreturn]] void	run_server();
	/* Метод запускает сервер. Вызывает select() на серверные сокеты с бесконечным тайм-аутом,
	 * Проверяет в какой сокет поступило соединение и вызывает метод serve_client(), передавая туда сокет клиента */
};

#endif //WEBSERV_WEBSERV_HPP
