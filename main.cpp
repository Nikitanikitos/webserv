/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 18:05:50 by imicah            #+#    #+#             */
/*   Updated: 2020/11/22 02:04:56 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netinet/in.h>
#include <zconf.h>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>
#include "libft.hpp"

int main() {
	fd_set				sockets_set;
	fd_set				accepts_set;
	int					fd_socket;
	int					fd_accept;
	struct sockaddr_in	sock_addr;
	char				buf[128];
	int					bytes;
	std::string			request;
	struct in_addr		ip;

	ft_memset(buf, 0, 512);
	ft_memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = PF_INET;
	sock_addr.sin_port = ft_htons(80);
	sock_addr.sin_addr.s_addr = ft_htonl(INADDR_ANY);

	ip.s_addr = INADDR_ANY;
	char *q = inet_ntoa(ip);

	fd_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	int one = 1;
	setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	bind(fd_socket, (struct sockaddr *) &sock_addr, sizeof(sock_addr));

	fcntl(fd_socket, F_SETFL, O_NONBLOCK);
	FD_ZERO(&sockets_set);
	FD_ZERO(&accepts_set);
	FD_SET(fd_socket, &sockets_set);

	listen(fd_socket, 10);

	while (true) {
		select(fd_socket + 1, &sockets_set, NULL, NULL, NULL);

		fd_accept = accept(fd_socket, 0, 0);
		if (fd_accept > 0) {
			while (true) {
				ft_memset(buf, 0, 128);
				bytes = recv(fd_accept, buf, 128, 0);
				if (strncmp(buf, "\r\n", 2) == 0)
					break;

				request += buf;
			}
			send(fd_accept, request.c_str(), request.size(), 0);
			close(fd_accept);
			break;
		}
	}
//	exit(0);
	return (0);
}
