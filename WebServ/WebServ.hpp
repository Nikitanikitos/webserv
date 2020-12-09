/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:49:07 by nikita            #+#    #+#             */
/*   Updated: 2020/12/09 07:24:34 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_WEBSERV_HPP
# define WEBSERV_WEBSERV_HPP

# include <string>
# include <map>
# include <queue>
# include "VirtualServer.hpp"
# include "Client.hpp"
# include "exceptions.hpp"
# include "libft.hpp"
# include "ThreadPool.hpp"

class WebServ {
	friend void*	worker(void*);

private:
	std::vector<int>						_sockets;
	std::vector<Client*>					_clients;
	std::vector<VirtualServer>				_list_virtual_servers;

	int 									_number_workers;
	ThreadPool								_thread_pool;

	static void			_read_request(Client*);
	static void			_parsing_request(Client*);
	static void			_generate_response(Client*);
	static void			_send_response(Client*);
	static void			_close_connection(Client*);

	static void			_default_handler(Client *http_object, const VirtualServer& virtual_server, const Location& location);
	void				_cgi_handler(const Request&, const VirtualServer&, const Location&, int);
	void				_proxy_handler(const Request&, const VirtualServer&, const Location&, int);

	static void			_POST_method_handler(const Request& request, struct stat* buff, const VirtualServer& virtual_server);
	static void			_GET_HEAD_methods_handler(Client *http_object, struct stat* buff, const Location& location);

	static Response		_static_file_handler(const Request& request, const std::string& path_to_file);
	static Response		_autoindex_handler(const Request& request, const std::string& path_to_target);

	static std::string	_autoindex_generate(const Request& request, const std::string& path_to_target);

	void				_create_workers();

	[[nodiscard]] const VirtualServer& _get_virtual_server(Request *request) const;

	void		generate_request(Client *http_object);
	void		generate_response(Client *http_object);
	void		send_response(Client *http_object);

	static std::string		_get_path_to_target(const Request&, const Location&);


public:
	explicit WebServ(const std::vector<VirtualServer>&);
	/* Инициализируем _set_vs_sockets, добавляем все сокеты виртуальных серверов в _set_vs_sockets, */

	virtual ~WebServ();

	[[noreturn]] void	run_server();
	/* Метод запускает сервер. Вызывает select() на серверные сокеты с бесконечным тайм-аутом,
	 * Проверяет в какой сокет поступило соединение и вызывает метод serve_client(), передавая туда сокет клиента */

	void	set_number_workers(int number_workers);
};

#endif //WEBSERV_WEBSERV_HPP
