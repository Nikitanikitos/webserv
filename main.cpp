/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 18:05:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/20 15:48:09 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include "WebServ.hpp"
#include "ParseConfigFile.hpp"
#include <algorithm>
#include <iostream>

int		exit() {
	WebServ::working = 0;
	return (0);
}

int		main(int ac, char **av) {
	std::string					number_of_workers;
	try {
		ParseConfigFile				parse(((ac == 2) ? av[1] : (char*)"default.conf"));
		std::vector<VirtualServer*>	list_virtual_server = parse.ParseFile(number_of_workers);
		WebServ						server(ft_atoi(number_of_workers.c_str()));

		for (size_t i = 0; i < list_virtual_server.size(); ++i)
			server.addVirtualServer(list_virtual_server[i]);

		signal(SIGINT, exit);
		server.runServer();
	}
	catch (ParseConfigFile::ParseConfigFileException& e)
		{ std::cout << e.what() << std::endl;}
	return (0);
}
