/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 14:17:15 by nikita            #+#    #+#             */
/*   Updated: 2020/12/17 21:24:09 by imicah           ###   ########.fr       */
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

std::vector<std::string>	ParseConfigFile::_GetArgsFromLine(std::string &input) const {
	std::vector<std::string>	result;
	size_t						posFind;

	while (!input.empty()){
		posFind = input.find(' ');
		input.erase(posFind, input.find_first_not_of(' '));
		posFind = input.find(' ');
		result.push_back(input.substr(0, posFind));
		input.erase(0, posFind);
	}
	return (result);
}

int 			ParseConfigFile::_GetIndexOfArg(std::string const &arg, std::string *arr, int size) const {
	for (int index = 0; index < size; ++index)
		if (arr[index] == arg)
			return index;
	return (-1);
}

bool			ParseConfigFile::_CheckTabulation(const std::string &line, int expectedTabCount) const {
	for (int i = 0; i < expectedTabCount; ++i) {
		if (line.compare(TAB_SIZE * i, TAB_SIZE, TAB))
			return (false);
	}
	return line[expectedTabCount * TAB_SIZE] != ' ';
}

bool			ParseConfigFile::_CheckPort(int port) const { return (port <= 262143 && port >= 1024); }

VirtualServer	ParseConfigFile::_ParseVsDirective() {
	VirtualServer	virtualServer;
	std::string		line;

	while (!_line_surplus.empty() || ft_getline(_fd, line)) {
		if (!_line_surplus.empty()) {
			line = _line_surplus;
			_line_surplus.clear();
		}
		if (line.empty() || line[0] == '#')
			continue;
		else if (!_CheckTabulation(line, 1)) {
			_line_surplus = line;
			if (virtualServer.GetPort().empty() || virtualServer.GetIp().empty())
				throw ParseConfigFileException("Port and Host can not be empty");
			return (virtualServer);
		}
		std::vector<std::string> trimmedStr = _GetArgsFromLine(line);

		switch (_GetIndexOfArg(trimmedStr[0], server_current_fields, 6)) {
			case server_names_d:
				for (size_t i = 1; i < trimmedStr.size(); ++i)
					virtualServer.AddServerName(trimmedStr[i]);
				break;
			case error_page_d:
				if (trimmedStr.size() == 3)
					virtualServer.AddErrorPage(trimmedStr[1], trimmedStr[2]);
				else
					throw ParseConfigFileException("Wrong error page parameter");
				break;
			case limit_body_size_d:
				if (trimmedStr.size() == 2 && ONLY_DIGITS(trimmedStr[1]))
					virtualServer.SetLimitClientBodySize(std::stoi(trimmedStr[1]));
				else
					throw ParseConfigFileException("Wrong limit body size parameter");
				break;
			case host_d:
				if (trimmedStr.size() == 2)
					virtualServer.SetIp(trimmedStr[1]);
				else
					throw ParseConfigFileException("Wrong host parameter");
				break;
			case port_d:
				if (trimmedStr.size() == 2 && _CheckPort(stoi(trimmedStr[1])))
					virtualServer.SetPort(trimmedStr[1]);
				else
					throw ParseConfigFileException("Wrong port parameter");
				break;
			case location_d:
				if (trimmedStr.size() == 2)
					virtualServer.AddLocation(_ParseLocationDirective(trimmedStr[1]));
				else
					throw ParseConfigFileException("Wrong location path parameter");
				break;
			default:
				throw ParseConfigFileException("Unknown parameter");
		}
	}
	if (virtualServer.GetPort().empty() || virtualServer.GetIp().empty())
		throw ParseConfigFileException("Port and Host can not be empty");
	return (virtualServer);
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

std::string&		ParseConfigFile::_CheckLocationPath(std::string &path) const {
	if (path[0] != '/')
		path.insert(path.begin(), '/');
	return (path);
}

Location			ParseConfigFile::_ParseLocationDirective(std::string &locationAttribute) {
	Location        location;
	std::string     line;

	location.SetPath(_CheckLocationPath(locationAttribute));
	while (ft_getline(_fd, line)) {
		if (line.empty() || line[0] == '#')
			continue;
		else if (!_CheckTabulation(line, 2)) {
			_line_surplus = line;
			return (location);
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

std::vector<VirtualServer>		ParseConfigFile::ParseFile(std::string& numberOfWorkers) {
	if ((_fd = open(_filename, O_RDONLY)) < 0)
		throw ParseConfigFileException("Config file can not be opened");
	std::string line;
	std::vector<VirtualServer> virtualServers;
	while (!_line_surplus.empty() || ft_getline(_fd, line)) {
		if (!_line_surplus.empty()) {
			line = _line_surplus;
			_line_surplus.clear();
		}
		if (line[0] == '#')
			continue;
		else if (!line.compare(0, 7, "worker "))
			numberOfWorkers.append(&line[7]);
		else if (line == "server") {
			VirtualServer	virtual_server = _ParseVsDirective();
			virtual_server.SortServerNames();
			if (_CheckCorrectVS(virtual_server, virtualServers))
				virtualServers.push_back(virtual_server);
			else
				throw ParseConfigFileException("Server already exists " + line);
		}
		else if (line.length() > 0)
			throw ParseConfigFileException("Unknown parameter " + line);
	}
	close(_fd);
	return (virtualServers);
}

bool	ParseConfigFile::_CheckCorrectVS(const VirtualServer& virtual_server, const std::vector<VirtualServer> list_virtual_server) {
	for (int i = 0; i < list_virtual_server.size(); ++i)
		if (virtual_server == list_virtual_server[i])
			return (false);
	return (true);
}
