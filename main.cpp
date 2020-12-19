/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 18:05:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/18 22:55:49 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WebServ.hpp>
#include <ParseConfigFile.hpp>

int		main(int ac, char **av, char **env) {
	std::string					number_of_workers;
	ParseConfigFile				parse(((ac == 2) ? av[1] : (char*)"default_for_linux.conf"));
	std::vector<VirtualServer*>	list_virtual_server = parse.ParseFile(number_of_workers);
	WebServ						server(std::stoi(number_of_workers));

	for (int i = 0; i < list_virtual_server.size(); ++i)
		server.AddVirtualServer(list_virtual_server[i]);

	server.RunServer();
	return (0);
}
