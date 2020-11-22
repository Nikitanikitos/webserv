/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:07 by nikita            #+#    #+#             */
/*   Updated: 2020/11/22 14:20:45 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_WEBSERV_HPP
# define WEBSERV_WEBSERV_HPP

# include <string>
# include <map>
# include <zconf.h>
# include "VirtualServer.hpp"

class WebServ {
private:
	fd_set							_set_vs_sockets;
	std::vector<VirtualServer>		_list_virtual_servers;

	std::map<std::string, std::string>	_check_request_header();
	/* Метод парсит заголовок из request, сохраняет их в словарь и возвращает */

	void	_execute_cgi_client();

	void	_parse_request_body(); // TODO что метод возращает и возвращает ли вообще?
	/* Метод парсит тело запроса */

	void	_get_request(VirtualServer&);
	/* Метод получает данные запроса через функцию recv().
	 * Из первой строки получает метод, путь и заносит в объект Request.
	 * Далее парсит заголовки, заносит их в словарь и проверяет корректность заголовка через _check_request_header().
	 * Далее парсит тело запроса через _parse_request_body() (если тело запроса есть).
	 * Возвращает объект Request() */

	void	_give_response();
	/* Принимает объект Request(), создает объект Response(), составляет тело ответа и отсылает его через send() */

public:
	explicit WebServ(const std::vector<VirtualServer>&);
	/* Инициализируем _set_vs_sockets, добавляем все сокеты виртуальных серверов в _set_vs_sockets, */

	virtual ~WebServ();

	void	run_server();
	/* Метод запускает сервер. Вызывает select() на серверные сокеты с бесконечным тайм-аутом,
	 * Проверяет в какой сокет поступило соединение и вызывает метод serve_client(), передавая туда сокет клиента */

	void	serve_client(int);
	/* Метод вызывает функцию accept() на сокет и вызывает метод _get_request(),
	 * который возвращает объект Request.
	 * вызов метода _give_response() */
};

#endif //WEBSERV_WEBSERV_HPP
