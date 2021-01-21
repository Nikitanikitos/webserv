/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 14:17:15 by nikita            #+#    #+#             */
/*   Updated: 2020/12/21 12:40:42 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseConfigFile.hpp"

# define ONLY_DIGITS(s)	(s.find_first_not_of("0123456789") == std::string::npos)
# define TAB			"    "
# define TAB_SIZE		4

std::string ParseConfigFile::server_current_fields[virtual_server_directive] = {
		"server_names",
		"error_page",
		"host",
		"port",
		"location"
};

std::string ParseConfigFile::location_current_fields[location_directive] = {
		"allow_methods",
		"root",
		"autoindex",
		"index",
		"cgi_pass",
		"limit_client_body_size",
};

std::vector<std::string>	ParseConfigFile::getArgsFromLine(std::string &input) const {
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

int 			ParseConfigFile::getIndexOfArg(std::string const &arg, std::string *arr, int size) const {
	for (int index = 0; index < size; ++index)
		if (arr[index] == arg)
			return index;
	return (-1);
}

bool			ParseConfigFile::checkTabulation(const std::string &line, int expectedTabCount) const {
	for (int i = 0; i < expectedTabCount; ++i) {
		if (line.compare(TAB_SIZE * i, TAB_SIZE, TAB))
			return (false);
	}
	return line[expectedTabCount * TAB_SIZE] != ' ';
}

bool			ParseConfigFile::checkPort(int port) const { return (port <= 262143 && port >= 1024); }

VirtualServer*	ParseConfigFile::parseVsDirective() {
	VirtualServer*	virtualServer = new VirtualServer();
	std::string		line;

	while (!line_surplus.empty() || ft_getline(fd, line)) {
		if (!line_surplus.empty()) {
			line = line_surplus;
			line_surplus.clear();
		}
		if (line.empty() || line[0] == '#')
			continue;
		else if (!checkTabulation(line, 1)) {
			line_surplus = line;
			break ;
		}
		std::vector<std::string> trimmedStr = getArgsFromLine(line);

		switch (getIndexOfArg(trimmedStr[0], server_current_fields, virtual_server_directive)) { //TODO хуй знает про find
			case server_names_d:
				for (size_t i = 1; i < trimmedStr.size(); ++i) {
					if (std::find(virtualServer->getServerNames().begin(), virtualServer->getServerNames().end(), trimmedStr[i]) != virtualServer->getServerNames().end())
						throw ParseConfigFileException("Bad config, double server name");
					virtualServer->addServerName(trimmedStr[i]);
				}
				break;
			case error_page_d:
				if (trimmedStr.size() == 3) {
					if (!virtualServer->getErrorPage(trimmedStr[1]).empty())
						throw ParseConfigFileException("Bad config, double error page");
					virtualServer->addErrorPage(trimmedStr[1], trimmedStr[2]);
				}
				else
					throw ParseConfigFileException("Wrong error page parameter");
				break;
			case host_d:
				if (trimmedStr.size() == 2) {
					if (!virtualServer->getHost().empty())
						throw ParseConfigFileException("Bad config, double host");
					virtualServer->setHost(trimmedStr[1]);
				}
				else
					throw ParseConfigFileException("Wrong host parameter");
				break;
			case port_d:
				if (trimmedStr.size() == 2 && checkPort(ft_atoi(trimmedStr[1].c_str())))
					virtualServer->setPort(trimmedStr[1]);
				else
					throw ParseConfigFileException("Wrong port parameter");
				break;
			case location_d:
				if (trimmedStr.size() == 2)
					virtualServer->addLocation(parseLocationDirective(trimmedStr[1]));
				else
					throw ParseConfigFileException("Wrong location path parameter");
				break;
			default:
				throw ParseConfigFileException("Unknown parameter");
		}
	}
	if (virtualServer->getPort().empty() || virtualServer->getHost().empty())
		throw ParseConfigFileException("Port and Host can not be empty");
	return (virtualServer);
}

void				ParseConfigFile::addAllowMethodsToLocation(Location *location, const std::vector<std::string>& trimmedStr) {
	if (trimmedStr.size() == 1)
		throw ParseConfigFileException("Allow methods must contain at least 1 parameter");
	location->eraseAcceptedMethods();
	for (size_t i = 1; i < trimmedStr.size(); ++i) {
		if (trimmedStr[i] == "GET")
			location->addAllowMethod(GET);
		else if (trimmedStr[i] == "HEAD")
			location->addAllowMethod(HEAD);
		else if (trimmedStr[i] == "POST")
			location->addAllowMethod(POST);
		else if (trimmedStr[i] == "PUT")
			location->addAllowMethod(PUT);
		else
			throw ParseConfigFileException("Unknown allow method parameter");
	}
}

void				ParseConfigFile::setAutoindexInLocation(Location *location, const std::vector<std::string>& trimmedStr) {
	if (trimmedStr.size() != 2)
		throw ParseConfigFileException("Autoindex has no parameter");
	if (trimmedStr[1] == "on")
		location->setAutoindex(true);
	else if (trimmedStr[1] == "off")
		location->setAutoindex(false);
	else
		throw ParseConfigFileException("Unknown option to autoindex");
}

std::string&		ParseConfigFile::checkLocationPath(std::string &path) const {
	if (path[0] != '/')
		path.insert(path.begin(), '/');
	else if (path.size() > 1 && path[path.size() - 1] == '/')
		path.erase(--path.end());
	return (path);
}

Location*			ParseConfigFile::parseLocationDirective(std::string &locationAttribute) {
	Location*		location = new Location();
	std::string		line;

	location->setPath(checkLocationPath(locationAttribute));
	while (ft_getline(fd, line)) {
		if (line.empty() || line[0] == '#')
			continue;
		else if (!checkTabulation(line, 2)) {
			line_surplus = line;
			return (location);
		}
		std::vector<std::string> trimmedStr = getArgsFromLine(line);

		switch (getIndexOfArg(trimmedStr[0], location_current_fields, location_directive)) {
			case allow_methods_d:
				addAllowMethodsToLocation(location, trimmedStr);
				break;
			case root_d:
				if (trimmedStr.size() != 2)
					throw ParseConfigFileException("Location root has no parameter");
				location->setRoot(trimmedStr[1]);
				break;
			case autoindex_d:
				setAutoindexInLocation(location, trimmedStr);
				break;
			case index_d:
				if (trimmedStr.size() != 2)
					throw ParseConfigFileException("Index has no parameter");
				location->setIndex(trimmedStr[1]);
				break;
			case cgi_pass_d: // TODO изменить парсинг cgi вот здесь
				if (trimmedStr.size() != 3)
					throw ParseConfigFileException("Cgi_pass has no parameter");
				location->addCgi(trimmedStr[1], trimmedStr[2]);
				break;
			case limit_client_body_size_d:
				if (trimmedStr.size() == 2 && ONLY_DIGITS(trimmedStr[1]))
					location->setLimitClientBodySize(ft_atoi(trimmedStr[1].c_str()));
				else
					throw ParseConfigFileException("Wrong limit body size parameter");
				break;
			default:
				throw ParseConfigFileException("Unknown parameter " + trimmedStr[0]);
		}
	}
	return (location);
}

std::vector<VirtualServer*>		ParseConfigFile::ParseFile(std::string& numberOfWorkers) {
	std::string					line;
	std::vector<VirtualServer*>	virtualServers;

	if ((fd = open(filename, O_RDONLY)) < 0)
		throw ParseConfigFileException("Config file can not be opened");
	while (!line_surplus.empty() || ft_getline(fd, line)) {
		if (!line_surplus.empty()) {
			line = line_surplus;
			line_surplus.clear();
		}
		if (line[0] == '#')
			continue;
		else if (!line.compare(0, 7, "worker "))
			numberOfWorkers.append(&line[7]);
		else if (line == "server")
			AddVirtualServer(line, virtualServers);
		else if (line.length() > 0)
			throw ParseConfigFileException("Unknown parameter " + line);
	}
	close(fd);
	return (virtualServers);
}

void	ParseConfigFile::AddVirtualServer(const std::string& line, std::vector<VirtualServer*>& virtualServers) {
	VirtualServer*	virtual_server = parseVsDirective();

	virtual_server->sortServerNames();
	if (checkCorrectVs(virtual_server, virtualServers))
		virtualServers.push_back(virtual_server);
	else
		throw ParseConfigFileException("Server already exists " + line);
}

bool	ParseConfigFile::checkCorrectVs(const VirtualServer *virtual_server,
										const std::vector<VirtualServer*>& list_virtual_server) {
	for (size_t i = 0; i < list_virtual_server.size(); ++i)
		if (*virtual_server == *(list_virtual_server[i])) return (false);
	return (true);
}
