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

# define ONLY_DIGITS(s) (s.find_first_not_of("0123456789") == std::string::npos)
# define TAB "    "
# define TAB_SIZE 4

std::string ParseConfigFile::serverCurrentFields[6] = {
		"server_names",
		"error_page",
		"limit_body_size",
		"host",
		"port",
		"location"
};

std::string ParseConfigFile::locationCurrentFields[7] = {
		"allow_methods",
		"root",
		"autoindex",
		"index",
		"cgi_pass",
		"extension",
		"proxy_pass"
};

ParseConfigFile::ParseConfigFile(char *filename) :_filename(filename), _line_surplus(std::string()) { }

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

int 			ParseConfigFile::_getIndexOfArg(std::string const &arg, std::string *arr, int size) const {
	for (int index = 0; index < size; ++index)
		if (arr[index] == arg)
			return index;
	return -1;
}

bool ParseConfigFile::_checkTabulation(const std::string &line, int expectedTabCount) const {
	for (int i = 0; i < expectedTabCount; ++i) {
		if (line.compare(TAB_SIZE * i, TAB_SIZE, TAB))
			return false;
	}
	// TODO refactor
	return line[expectedTabCount * TAB_SIZE] == ' ';
}

VirtualServer	ParseConfigFile::_parse_vs_directive() {
	/* Метод будет возвращать объект класса VirtualServer со всеми инициализированными полями */
	VirtualServer virtual_server;
	std::string line;

	while (!_line_surplus.empty() || ft_getline(_fd, line)) {
		if (!_line_surplus.empty()) {
			line = _line_surplus;
			_line_surplus.clear();
		}
		std::vector<std::string> trimmedStr = _getArgsFromLine(line);
		if (trimmedStr.empty() || trimmedStr[0][0] == '#')
			continue;
		else if (!_checkTabulation(line, 1)) {
			_line_surplus = line;
			return virtual_server;
		}
		// TODO break loop if tabsize is 0 and call this func again
		switch (_getIndexOfArg(trimmedStr[0], serverCurrentFields, 6)) {
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
				if (trimmedStr.size() == 2 && ONLY_DIGITS(trimmedStr[1])) {
					virtual_server.set_limit_client_body_size(std::stoi(trimmedStr[1]));
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
				// TODO multiple ports
				if (trimmedStr.size() == 2)
					virtual_server.add_port(trimmedStr[1]);
				else
					throw std::exception(); // TODO error
			}
			case 5: { // location
				if (trimmedStr.size() == 2)
					virtual_server.add_location(_parse_location_directive(trimmedStr[1]));
				else
					throw std::exception(); // TODO error
			}
			default:
				throw std::exception(); // TODO error invalid field in config
		}
	}
	return virtual_server;
}

Location			ParseConfigFile::_parse_location_directive(std::string const &locationAttribute) {
	/* Метод будет возвращать объект класса Route со всеми инициализированными полями */

	Location location;
	std::string line;
	location.set_path(locationAttribute);
	while (ft_getline(_fd, line)) {
		std::vector<std::string> trimmedStr = _getArgsFromLine(line);
		if (trimmedStr.empty() || trimmedStr[0][0] == '#')
			continue;
		else if (!_checkTabulation(line, 2)) {
			_line_surplus = line;
			return location;
		}
		switch (_getIndexOfArg(trimmedStr[0], locationCurrentFields, 7)) {
			case 0: { // allow_methods
				if (trimmedStr.size() == 1)
					throw std::exception(); // todo error
				location.erase_accepted_methods();
				for (int i = 1; i < trimmedStr.size(); ++i) {
					if (trimmedStr[i] == "GET")
						location.add_accepted_method(GET);
					else if (trimmedStr[i] == "HEAD")
						location.add_accepted_method(HEAD);
					else if (trimmedStr[i] == "POST")
						location.add_accepted_method(POST);
					else if (trimmedStr[i] == "PUT")
						location.add_accepted_method(PUT);
					else if (trimmedStr[i] == "DELETE")
						location.add_accepted_method(DELETE);
					else if (trimmedStr[i] == "OPTIONS")
						location.add_accepted_method(OPTIONS);
					else
						throw std::exception(); // TODO error
				}
			}
			case 1: { // root
				if (trimmedStr.size() != 2)
					throw std::exception(); // todo error
				location.set_root(trimmedStr[1]);
			}
			case 2: { // autoindex
				if (trimmedStr.size() != 2)
					throw std::exception(); // todo error
				if (trimmedStr[1] == "on")
					location.set_autoindex(true);
				else if (trimmedStr[1] == "off")
					location.set_autoindex(false);
				else
					throw std::exception(); // todo error
			}
			case 3: { // index
				if (trimmedStr.size() != 2)
					throw std::exception(); // todo error
				location.set_index(trimmedStr[1]);
			}
			case 4: { // cgi_pass
				if (trimmedStr.size() != 2)
					throw std::exception(); // todo error
				location.set_location_type(cgi);
			}
			case 5: { // extension
				if (trimmedStr.size() != 2)
					throw std::exception(); // todo error
				// location.set_extension(trimmedStr[1]);
			}
			case 6: { // proxy_pass
				if (trimmedStr.size() != 2)
					throw std::exception(); // todo error
				location.set_location_type(proxy);
			}
			default:
				throw std::exception(); // TODO error
		}
	}
	return (location);
}

std::vector<VirtualServer> ParseConfigFile::parse_file() {
	/* метод будет возвращать список виртуальных серверов, в которых есть список роутеров */
	if ((_fd = open(_filename, O_RDONLY)) < 0)
		throw std::exception(); // TODO ERROR
	std::string line;
	std::vector<VirtualServer> virtual_servers;
	while (!_line_surplus.empty() || ft_getline(_fd, line)) {
		if (!_line_surplus.empty()) {
			line = _line_surplus;
			_line_surplus.clear();
		}
		if (line[0] == '#')
			continue;
		else if (line.compare(0, 7, "worker ")) {
			int workers = std::stoi(&line[7]); // atoi workers
			std::cout << "Workers = " << workers << std::endl;
		}
		else if (line == "server")
			virtual_servers.push_back(_parse_vs_directive()); // mb check return value ? or not
		else if (line.length() > 0)
			throw std::exception(); // TODO ERROR
	}
	return (virtual_servers);
}
