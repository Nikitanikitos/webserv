/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 18:05:50 by imicah            #+#    #+#             */
/*   Updated: 2021/01/26 19:31:55 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include "WebServ.hpp"
#include "ParseConfigFile.hpp"
#include <algorithm>
#include <iostream>

void	exit_(int signum) {
	if (signum == SIGINT || signum == SIGTERM) {
		std::cout << "See you soon!" << std::endl;
		WebServ::working = 0;
	}
	write(WebServ::imaginary_pipe[1], "1", 1);
}

int		main(int ac, char **av) {
	signal(SIGINT, exit_);
	signal(SIGTERM, exit_);
	try {
		std::string					number_of_workers;
		ParseConfigFile				parse(((ac == 2) ? av[1] : (char*)"default.conf"));
		std::vector<VirtualServer*>	list_virtual_server = parse.ParseFile(number_of_workers);
		WebServ						server(ft_atoi(number_of_workers.c_str()));

		for (size_t i = 0; i < list_virtual_server.size(); ++i)
			server.addVirtualServer(list_virtual_server[i]);

		std::cout << "Webserv running!" << std::endl;
		server.runServer();
	}
	catch (ParseConfigFile::ParseConfigFileException& e)
		{ std::cerr << e.what() << std::endl;}
	exit(0);
}
