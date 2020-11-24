/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 18:05:50 by imicah            #+#    #+#             */
/*   Updated: 2020/11/24 03:07:14 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netinet/in.h>
#include <zconf.h>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>
#include <signal.h>
#include "libft.hpp"

int main(int ac, char **av, char **env) {
	fd_set				sockets_set;
	int					fd_socket_80 = 1;
	int					fd_socket_8080;
	int					fd_accept;
	struct sockaddr_in	sock_addr;
	char				buf[128];
	int					bytes;
	std::string			request;

//	ft_memset(buf, 0, 512);
	FD_ZERO(&sockets_set);
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
//	ft_memset(&sock_addr, 0, sizeof(sock_addr));
//	sock_addr.sin_family = PF_INET;
//	sock_addr.sin_port = ft_htons(8080);
//	sock_addr.sin_addr.s_addr = ft_htonl(INADDR_ANY);
//
//	fd_socket_8080 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//	bind(fd_socket_8080, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
//	fcntl(fd_socket_8080, F_SETFL, O_NONBLOCK);
//
	FD_SET(fd_socket_80, &sockets_set);

	while (1) {
		select(fd_socket_80 + 1, nullptr, &sockets_set, nullptr, nullptr);

		write(fd_socket_80, "wqe", 3);
	}

	//	FD_SET(fd_socket_8080, &sockets_set);
//
//	listen(fd_socket_80, 10);
//	listen(fd_socket_8080, 10);

	int 	pid = getpid();
	std::cout << "eqw" << std::endl;
	kill(pid, SIGSTOP);
	while (true) {
		select(fd_socket_8080 + 1, &sockets_set, nullptr, nullptr, nullptr);
		std::cout << "eqw" << std::endl;

		fd_accept = accept(fd_socket_80, nullptr, nullptr);
		if (fd_accept > 0) {
			while (true) {
				ft_memset(buf, 0, 128);
				bytes = recv(fd_accept, buf, 128, 0);
				if (strncmp(buf, "\r\n", 2) == 0 || bytes == 0)
					break;
				else if (!strcmp(buf, "exit\r\n"))
					exit(0);

				send(fd_accept, buf, bytes, 0);
			}
			close(fd_accept);
//			break;
		}
	}
	return (0);
}
