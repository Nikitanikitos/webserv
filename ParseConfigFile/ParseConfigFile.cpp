/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 14:17:15 by nikita            #+#    #+#             */
/*   Updated: 2020/11/25 04:21:46 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseConfigFile.hpp"

ParseConfigFile::ParseConfigFile(char *filename) :_filename(filename) { }


VirtualServer	ParseConfigFile::_parse_vs_directive() {
	/* Метод будет возвращать объект класса VirtualServer со всеми инициализированными полями */
	VirtualServer virtual_server;
	
	return (VirtualServer());
}

Location			ParseConfigFile::_parse_location_directive() {
	/* Метод будет возвращать объект класса Route со всеми инициализированными полями */
	return (Location());
}

std::vector<VirtualServer> ParseConfigFile::parse_file() {
	/* метод будет возвращать список виртуальных серверов, в которых есть список роутеров */
	if ((_fd = open(_filename, O_RDONLY)) < 0)
		throw std::exception(); // ERROR
	char *line_char;
	std::string line_str;
	int gnl_return;
	std::vector<VirtualServer> virtual_servers;
	while ((gnl_return = ft_getline(_fd, &line_char)) > 0) {
		line_str = line_char;
		free(line_char);
		line_char = nullptr;
		// check is worker

		// remove spaces
		std::string::iterator end_pos = std::remove(line_str.begin(), line_str.end(), ' ');
		line_str.erase(end_pos, line_str.end());
		if (line_str == "#")
			continue;
		else if (line_str == "server{")
			virtual_servers.push_back(_parse_vs_directive()); // mb check return value ? or not
		else if (line_str.length() > 0)
			throw std::exception(); // ERROR
	}
	if (gnl_return < 0)
		throw std::exception(); // ERROR
	return (std::vector<VirtualServer>());
}
