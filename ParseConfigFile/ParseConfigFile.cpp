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

std::vector<std::string>	getArgsFromLine(std::string const &input) {
	std::vector<std::string>	result;

	for (size_t pos = 0; pos < input.length(); pos++) {
		size_t posFind = input.find_first_not_of(' ', pos);
		if (posFind == std::string::npos)
			return result;
		if ((pos = input.find_first_of(' ', pos)) == std::string::npos)
			pos = input.length();
		result.push_back(input.substr(posFind, (pos - posFind)));
	}
	return result;
}

VirtualServer	ParseConfigFile::_parse_vs_directive() {
	/* Метод будет возвращать объект класса VirtualServer со всеми инициализированными полями */
	VirtualServer virtual_server;
	std::string line;
	//	server_names        name, ... (необязательный аргумент, по умолчанию используется port);
	//	error_page          status_code filename (необязательный аргумент, по умолчанию используется заготовленный файл);
	//	limit_body_size     size (необязательный аргумент, по умолчанию используется 1000);
	//	error_page          status_code filename (необязательный аргумент, по умолчанию используется заготовленный файл);
	//	host                host (обязательный аргумент);
	//	port                port (обязательный аргумент);


//	TODO add support and validate ";"
	while (ft_getline(_fd, line)) {
		// skip spaces
		size_t i = 0;
		for (i; i < line.length() && line[i] == ' '; ++i);
		if (line.compare(i, 4, "host")) {
			i += 4;
			for (i; i < line.length() && line[i] == ' '; ++i);
			virtual_server.set_host(line.substr(i, line.find(';')));
		}
		else if (line.compare(i, 4, "port")) {
			i += 4;
			for (i; i < line.length() && line[i] == ' '; ++i);
			virtual_server.add_port(line.substr(i, line.find(';')));
		}
		else if (line.compare(i, 12, "server_names")) {
			i += 12;
			for (i; i < line.length() && line[i] == ' '; ++i);
			for (size_t j = i; j < line.length(); ++j) {
				if (j == ',') {
					virtual_server.add_server_name(line.substr(i, j - i));
					i = j + 1;
				}
			}
		}
		else if (line.compare(i, 15, "limit_body_size")) {
			i += 15;
			virtual_server.set_limit_client_body_size(std::stoi(line, &i));
		}
		else if (line.compare(i, 10, "error_page")) {
			i+= 10;
			// TODO add error_page
		}
		else if (line.compare(i, 8, "location")) {
			i += 8;
			for (size_t j = i; j < line.length(); ++j) {
				if (j == '{') {
					// TODO validate and	asqd set <path> or <extension>
					virtual_server.add_location(_parse_location_directive());
					break;
				}
			}
			// call _parse_location_directive
		}
	}
	return (virtual_server);
}

Location			ParseConfigFile::_parse_location_directive() {
	/* Метод будет возвращать объект класса Route со всеми инициализированными полями */
//	location <path> {
//			allow_methods           METHOD_NAME, ... (обязательный аругмент);
//			root                    directory_path (обазятельный аргумент);
//			autoindex               on/off (необязательный аругмент, по умолчанию off);
//			request_is_directory    filename (необязательный аргумент, по умолчанию используется заготовленный файл);
//	}
//
//	location <extension> {
//			allow_methods           METHOD_NAME, ... (обязательный аругмент);
//			cgi_pass                path_to_cgi (обязательный аругмент);
//			root                    directory_path (обазятельный аргумент);
//	}
//
//	location <path> {
//			root                    directory_path (обазятельный аргумент);
//			extension               .php
//			accepted_methods        METHOD_NAME, ... (обязательный аругмент);
//			cgi_pass                path_to_cgi (обязательный аругмент);
//	}
//
//	location <path> {
//			accepted_methods        METHOD_NAME, ... (обязательный аругмент);
//			proxy_pass              URI (обязательный аругмент);
//	}
	Location location;
	std::string line;
	while (ft_getline(_fd, line)) {
		size_t i = 0;
		for (i; i < line.length() && line[i] == ' '; ++i);
		if (line.compare(i, 4, "root")) {
			i += 4;
			for (i; i < line.length() && line[i] == ' '; ++i);
			// TODO add root

		}
		else if (line.compare(i, 9, "extension")) {
			i += 9;
			// TODO add extension

		}
		else if (line.find('{'))
			break ;
	}
	return (location);
}

std::vector<VirtualServer> ParseConfigFile::parse_file() {
	/* метод будет возвращать список виртуальных серверов, в которых есть список роутеров */
	if ((_fd = open(_filename, O_RDONLY)) < 0)
		throw std::exception(); // ERROR
	std::string line;
	std::vector<VirtualServer> virtual_servers;
	while (ft_getline(_fd, line)) {
		// remove spaces
		std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
		line.erase(end_pos, line.end());
		if (line == "#")
			continue;
		else if (line.compare(0, 6, "worker")) {
			int workers = std::stoi(&line[6]); // atoi workers
		}
		else if (line == "server{")
			virtual_servers.push_back(_parse_vs_directive()); // mb check return value ? or not
		else if (line.length() > 0)
			throw std::exception(); // ERROR
	}
	return (std::vector<VirtualServer>());
}
