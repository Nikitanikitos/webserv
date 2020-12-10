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

std::string ParseConfigFile::serverCurrentFields[6] = {
		"server_names",
		"error_page",
		"limit_body_size",
		"host",
		"port",
		"location"
};

ParseConfigFile::ParseConfigFile(char *filename) :_filename(filename) { }

std::vector<std::string>	ParseConfigFile::_getArgsFromLine(std::string const &input) const {
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

int 			ParseConfigFile::_getIndexOfArg(std::string const &arg) const {
	for (int index = 0; index < 6; ++index)
		if (serverCurrentFields[index] == arg)
			return index;
	return -1;
}

bool 			ParseConfigFile::_manageSemicolon(std::vector<std::string>& v) {
	int countSemicolon = 0;
	for (size_t i = 0; i < v.size(); ++i)
		if (v[i].find(';'))
			++countSemicolon;
	if (countSemicolon == 1 && v[v.size() - 1][v[v.size() - 1].length() - 1] == ';') {
		v[v.size() - 1] = v[v.size() - 1].substr(0, v[v.size() - 1].size() - 1);
		return true;
	} else
		return false;
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


	while (ft_getline(_fd, line)) {
		std::vector<std::string> trimmedStr = _getArgsFromLine(line);
		if (trimmedStr.empty() || trimmedStr[0][0] == '#')
			continue;
		else if (trimmedStr.size() == 1)
			throw std::exception(); // TODO error in config file
		else if (!_manageSemicolon(trimmedStr))
			throw std::exception(); // TODO error in config file
		switch (_getIndexOfArg(trimmedStr[0])) {
			case 0: // server_names
				for (size_t i = 1; i < trimmedStr.size(); ++i)
					virtual_server.add_server_name(trimmedStr[i]);
			case 1: { // error_page
				if (trimmedStr.size() == 3)
					virtual_server.add_error_page(trimmedStr[1], trimmedStr[2]);
				else
					throw std::exception(); // TODO error in config file
				// trimmedStr[1] - код
				// trimmedStr[2] - адрес
			}
			case 2: { // limit_body_size
				if (trimmedStr.size() == 2) {
					try {
						virtual_server.set_limit_client_body_size(std::stoi(trimmedStr[1]));
					} catch (std::exception &e) {
						throw e; // TODO error
					}
				}
				else
					throw std::exception(); // TODO error
			}
			case 3: { // host
				if (trimmedStr.size() == 2)
					virtual_server.set_host(trimmedStr[1]);
				else
					throw std::exception(); // TODO error
			}
			case 4: { // port
				if (trimmedStr.size() == 2)
					virtual_server.add_port(trimmedStr[1]);
				else
					throw std::exception(); // TODO error
			}
			case 5: { // location
			}
			default:
				throw std::exception(); // TODO error invalid field in config
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
		throw std::exception(); // TODO ERROR
	std::string line;
	std::vector<VirtualServer> virtual_servers;
	while (ft_getline(_fd, line)) {
		if (line[0] == '#')
			continue;
		else if (line.compare(0, 7, "worker ")) {
			int workers = std::stoi(&line[7]); // atoi workers
			std::cout << "Workers = " << workers << std::endl;
		}
		else if (line == "server {")
			virtual_servers.push_back(_parse_vs_directive()); // mb check return value ? or not
		else if (line.length() > 0)
			throw std::exception(); // TODO ERROR
	}
	return (virtual_servers);
}
