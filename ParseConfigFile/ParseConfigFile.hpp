/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:50:51 by imicah            #+#    #+#             */
/*   Updated: 2020/12/18 16:28:36 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_PARSECONFIGFILE_HPP
# define WEBSERV_PARSECONFIGFILE_HPP

# include <string>
# include <iostream>
# include <fcntl.h>
# include "VirtualServer.hpp"
# include "Location.hpp"
# include "libft.hpp"

enum {
    server_names_d,
    error_page_d,
    limit_body_size_d,
    host_d,
    port_d,
    location_d
};

enum {
    allow_methods_d,
    root_d,
    autoindex_d,
    index_d,
    cgi_pass_d,
    extension_d,
};

class	ParseConfigFile {
private:
	std::string		_line_surplus;
	char*           _filename;
	int 			_fd;

	VirtualServer * _ParseVsDirective();

	Location * _ParseLocationDirective(std::string &locationAttribute);

	std::vector<std::string>	_GetArgsFromLine(std::string &input) const;

	int 						_GetIndexOfArg(std::string const &arg, std::string *arr, int size) const;

	bool 						_CheckTabulation(std::string const &line, int expectedTabCount) const;
	void 						_AddAllowMethodsToLocation(Location *location, const std::vector<std::string>& trimmedStr);
	void 						_SetAutoindexInLocation(Location *location, const std::vector<std::string>& trimmedStr);
	std::string&				_CheckLocationPath(std::string &path) const;
	bool 						_CheckPort(int port) const;

	bool						_CheckCorrectVS(const VirtualServer *virtual_server, const std::vector<VirtualServer*>& list_virtual_server);
public:
	explicit ParseConfigFile(char *filename)  : _filename(filename) { }
	~ParseConfigFile() { }

	std::vector<VirtualServer*> ParseFile(std::string &numberOfWorkers);

	static std::string		server_current_fields[6];
	static std::string		location_current_fields[7];

	class ParseConfigFileException: public std::exception {
	private:
		std::string 			_message;
	public:
		ParseConfigFileException(std::string const &message) : _message("Config File: " + message) { }

		virtual const char*		what() const throw() { return (_message.c_str()); }
	};

	void AddVirtualServer(const std::string& line, std::vector<VirtualServer*>& virtualServers);
};

#endif //WEBSERV_PARSECONFIGFILE_HPP
