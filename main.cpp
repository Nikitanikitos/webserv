/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 18:05:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/15 02:04:49 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netinet/in.h>
#include <zconf.h>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fstream>
#include <Location.hpp>
#include <VirtualServer.hpp>
#include <WebServ.hpp>
#include <ParseConfigFile.hpp>
#include "libft.hpp"

int main(int ac, char **av, char **env) {
	std::string			number_of_workers;
	ParseConfigFile		parse("default.conf");

	std::vector<VirtualServer>	list_virtual_server;

	list_virtual_server = parse.ParseFile(number_of_workers);

	WebServ			server(std::stoi(number_of_workers));

	for (int i = 0; i < list_virtual_server.size(); ++i)
		server.AddVirtualServer(list_virtual_server[i]);

	server.RunServer();
//	std::cout << __DATE__ << std::endl;
//	std::cout << __TIME__ << std::endl;
//	location.set_location_type(default_location);
//	location.set_autoindex(true);
//	location.set_root("/Users/imicah/CLionProjects/webserv");
//	location.set_index("index.html");
//
//	server.add_location(location);
//	server.set_host("127.0.0.1");
//	server.add_server_name("localhost");
//	server.add_port("8080");
//
//
//
//	WebServ		web_server(list_virtual_server);
//	web_server.set_number_workers(1);
//
//	web_server.RunServer();
	//	fd_set				sockets_set;
//	int					fd_socket_80 = 1;
//	int					fd_socket_8080;
//	int					fd_accept;
//	struct sockaddr_in	sock_addr;
//	char				buf[128];
//	int					bytes;
//	std::string			request;
//
//	ft_memset(buf, 0, 512);
//	FD_ZERO(&sockets_set);
//
//	ft_memset(&sock_addr, 0, sizeof(sock_addr));
//	sock_addr.sin_family = PF_INET;
//	sock_addr.sin_port = ft_htons(80);
//	sock_addr.sin_addr.s_addr = ft_htonl(INADDR_ANY);
//
//	fd_socket_80 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//	bind(fd_socket_80, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
//	fcntl(fd_socket_80, F_SETFL, O_NONBLOCK);
//
//		write(fd_socket_80, "wqe", 3);
//	}

//	FD_SET(fd_socket_8080, &sockets_set);
//
//	listen(fd_socket_80, 10);
//	listen(fd_socket_8080, 10);
//
//	while (true) {
//		select(fd_socket_8080 + 1, &sockets_set, nullptr, nullptr, nullptr);
//		std::cout << "eqw" << std::endl;
//
//		fd_accept = accept(fd_socket_80, nullptr, nullptr);
//		if (fd_accept > 0) {
//			while (true) {
//				ft_memset(buf, 0, 128);
//				bytes = recv(fd_accept, buf, 128, MSG_PEEK);
//				if (strncmp(buf, "\r\n", 2) == 0 || bytes == 0)
//					break;
//				else if (!strcmp(buf, "exit\r\n"))
//					exit(0);
//
//				write(fd_accept, "hhelo", 5);
//				send(fd_accept, buf, bytes, 0);
//			}
//			close(fd_accept);
////			break;
//		}
//	}
	return (0);
}
