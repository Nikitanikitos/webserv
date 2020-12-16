/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 18:05:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/16 13:52:52 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WebServ.hpp>
#include <ParseConfigFile.hpp>

int main(int ac, char **av, char **env) {
	std::string					number_of_workers;
	ParseConfigFile				parse("/Users/imicah/CLionProjects/webserv/default.conf");
	std::vector<VirtualServer>	list_virtual_server = parse.ParseFile(number_of_workers);
	WebServ						server(std::stoi(number_of_workers));

	for (int i = 0; i < list_virtual_server.size(); ++i)
		server.AddVirtualServer(list_virtual_server[i]);

	server.RunServer();
	return (0);
}
