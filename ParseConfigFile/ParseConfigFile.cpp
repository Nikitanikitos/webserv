/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 14:17:15 by nikita            #+#    #+#             */
/*   Updated: 2020/12/16 17:59:47 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseConfigFile.hpp"

# define ONLY_DIGITS(s) (s.find_first_not_of("0123456789") == std::string::npos)
# define TAB "    "
# define TAB_SIZE 4

std::string ParseConfigFile::server_current_fields[6] = {
		"server_names",
		"error_page",
		"limit_body_size",
		"host",
		"port",
		"location"
};

std::string ParseConfigFile::location_current_fields[7] = {
		"allow_methods",
		"root",
		"autoindex",
		"index",
		"cgi_pass",
		"extension",
		"proxy_pass"
};

ParseConfigFile::ParseConfigFile(char *filename) :_filename(filename), _line_surplus(std::string()) { }

ParseConfigFile::ParseConfigFileException::ParseConfigFileException(const std::string &message) : _message("Config File: " + message) {}

const char*					ParseConfigFile::ParseConfigFileException::what() const throw() {
	return _message.c_str();
}

std::vector<std::string>	ParseConfigFile::_GetArgsFromLine(std::string &input) const {
	std::vector<std::string>	result;
	size_t						pos_find;

	while (!input.empty()){
		pos_find = input.find(' ');
		input.erase(pos_find, input.find_first_not_of(' '));
		pos_find = input.find(' ');
		result.push_back(input.substr(0, pos_find));
		input.erase(0, pos_find);
	}
	return (result);
}

int 			ParseConfigFile::_GetIndexOfArg(std::string const &arg, std::string *arr, int size) const {
	for (int index = 0; index < size; ++index)
		if (arr[index] == arg)
			return index;
	return -1;
}

bool ParseConfigFile::_CheckTabulation(const std::string &line, int expectedTabCount) const {
	for (int i = 0; i < expectedTabCount; ++i) {
		if (line.compare(TAB_SIZE * i, TAB_SIZE, TAB))
			return false;
	}
	return line[expectedTabCount * TAB_SIZE] != ' ';
}

VirtualServer	ParseConfigFile::_ParseVsDirective() {
	VirtualServer virtual_server;
	std::string line;

	while (!_line_surplus.empty() || ft_getline(_fd, line)) {
		if (!_line_surplus.empty()) {
			line = _line_surplus;
			_line_surplus.clear();
		}
		if (line.empty() || line[0] == '#')
			continue;
		else if (!_CheckTabulation(line, 1)) {
			_line_surplus = line;
			return virtual_server;
		}
		std::vector<std::string> trimmedStr = _GetArgsFromLine(line);

		switch (_GetIndexOfArg(trimmedStr[0], server_current_fields, 6)) {
			case server_names_d:
				for (size_t i = 1; i < trimmedStr.size(); ++i)
					virtual_server.AddServerName(trimmedStr[i]);
				break;
			case error_page_d:
				if (trimmedStr.size() == 3)
					virtual_server.AddErrorPage(trimmedStr[1], trimmedStr[2]);
				else
					throw ParseConfigFileException("Wrong error page parameter");
				break;
			case limit_body_size_d:
				if (trimmedStr.size() == 2 && ONLY_DIGITS(trimmedStr[1]))
					virtual_server.SetLimitClientBodySize(std::stoi(trimmedStr[1]));
				else
					throw ParseConfigFileException("Wrong limit body size parameter");
				break;
			case host_d:
				if (trimmedStr.size() == 2)
					virtual_server.SetIp(trimmedStr[1]);
				else
					throw ParseConfigFileException("Wrong host parameter");
				break;
			case port_d:
				if (trimmedStr.size() > 1)
					for (int i = 1; i < trimmedStr.size(); ++i)
						virtual_server.SetPort(trimmedStr[i]);
				else
					throw ParseConfigFileException("Wrong port parameter");
				break;
			case location_d:
				if (trimmedStr.size() == 2)
					virtual_server.AddLocation(_ParseLocationDirective(trimmedStr[1]));
				else
					throw ParseConfigFileException("Wrong location path parameter");
				break;
			default:
				throw ParseConfigFileException("Unknown parameter");
		}
	}
	return virtual_server;
}

void				ParseConfigFile::_AddAllowMethodsToLocation(Location &location, const std::vector<std::string> &trimmedStr) {
	if (trimmedStr.size() == 1)
		throw ParseConfigFileException("Allow methods must contain at least 1 parameter");
	location.EraseAcceptedMethods();
	for (int i = 1; i < trimmedStr.size(); ++i) {
		if (trimmedStr[i] == "GET")
			location.AddAllowMethod(GET);
		else if (trimmedStr[i] == "HEAD")
			location.AddAllowMethod(HEAD);
		else if (trimmedStr[i] == "POST")
			location.AddAllowMethod(POST);
		else if (trimmedStr[i] == "PUT")
			location.AddAllowMethod(PUT);
		else if (trimmedStr[i] == "DELETE")
			location.AddAllowMethod(DELETE);
		else if (trimmedStr[i] == "OPTIONS")
			location.AddAllowMethod(OPTIONS);
		else
			throw ParseConfigFileException("Unknown allow method parameter");
	}
}

void				ParseConfigFile::_SetAutoindexInLocation(Location &location, const std::vector<std::string> &trimmedStr) {
	if (trimmedStr.size() != 2)
		throw ParseConfigFileException("Autoindex has no parameter");
	if (trimmedStr[1] == "on")
		location.SetAutoindex(true);
	else if (trimmedStr[1] == "off")
		location.SetAutoindex(false);
	else
		throw ParseConfigFileException("Unknown option to autoindex");
}

Location			ParseConfigFile::_ParseLocationDirective(std::string const &locationAttribute) {
	Location        location;
	std::string     line;

	location.SetPath(locationAttribute);
	while (ft_getline(_fd, line)) {
		if (line.empty() || line[0] == '#')
			continue;
		else if (!_CheckTabulation(line, 2)) {
			_line_surplus = line;
			return location;
		}
		std::vector<std::string> trimmedStr = _GetArgsFromLine(line);

		switch (_GetIndexOfArg(trimmedStr[0], location_current_fields, 7)) {
			case allow_methods_d:
				_AddAllowMethodsToLocation(location, trimmedStr);
				break;
			case root_d:
				if (trimmedStr.size() != 2)
					throw ParseConfigFileException("Location root has no parameter");
				location.SetRoot(trimmedStr[1]);
				break;
			case autoindex_d:
				_SetAutoindexInLocation(location, trimmedStr);
				break;
			case index_d:
				if (trimmedStr.size() != 2)
					throw ParseConfigFileException("Index has no parameter");
				location.SetIndex(trimmedStr[1]);
				break;
			case cgi_pass_d:
				if (trimmedStr.size() != 2)
					throw ParseConfigFileException("Cgi_pass has no parameter");
				location.SetLocationType(cgi_location);
				location.SetCgiPath(trimmedStr[1]);
				break;
			case extension_d:
				if (trimmedStr.size() != 2)
					throw ParseConfigFileException("Extension has no parameter");
				location.SetExtension(trimmedStr[1]);
				break;
			default:
				throw ParseConfigFileException("Unknown parameter " + trimmedStr[0]);
		}
	}
	return (location);
}

std::vector<VirtualServer>		ParseConfigFile::ParseFile(std::string& number_of_workers) {
	if ((_fd = open(_filename, O_RDONLY)) < 0)
		throw ParseConfigFileException("Config file can not be opened");
	std::string line;
	std::vector<VirtualServer> virtual_servers;
	while (!_line_surplus.empty() || ft_getline(_fd, line)) {
		if (!_line_surplus.empty()) {
			line = _line_surplus;
			_line_surplus.clear();
		}
		if (line[0] == '#')
			continue;
		else if (!line.compare(0, 7, "worker "))
			number_of_workers.append(&line[7]);
		else if (line == "server")
				virtual_servers.push_back(_ParseVsDirective());
		else if (line.length() > 0)
			throw ParseConfigFileException("Unknown parameter " + line);
	}
	return (virtual_servers);
}
