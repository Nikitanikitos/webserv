/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:50:51 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:37:06 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_PARSECONFIGFILE_HPP
# define WEBSERV_PARSECONFIGFILE_HPP

# include "VirtualServer.hpp"
# include "Location.hpp"

enum {
    server_names_d,
    error_page_d,
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
	limit_client_body_size_d,
};

enum {
	location_directive = 6,
	virtual_server_directive = 5
};

class	ParseConfigFile {
private:
	std::string		line_surplus;
	char*           filename;
	int 			fd;

	VirtualServer*				parseVsDirective();

	Location*					parseLocationDirective(std::string &locationAttribute);

	std::vector<std::string>	getArgsFromLine(std::string &input) const;

	int 						getIndexOfArg(std::string const &arg, std::string *arr, int size) const;

	bool 						checkTabulation(std::string const &line, int expectedTabCount) const;
	void 						addAllowMethodsToLocation(Location *location, const std::vector<std::string>& trimmedStr);
	void 						setAutoindexInLocation(Location *location, const std::vector<std::string>& trimmedStr);
	std::string&				checkLocationPath(std::string &path) const;
	inline bool 				checkPort(int port) const { return (port <= 262143 && port >= 1024); }
	bool						checkCorrectVs(const VirtualServer *virtual_server,
						  										const std::vector<VirtualServer*>& list_virtual_server);

public:
	explicit ParseConfigFile(char *filename)  : filename(filename), fd(0) { }
	~ParseConfigFile() { }

	std::vector<VirtualServer*> ParseFile(std::string &numberOfWorkers);

	void						AddVirtualServer(const std::string& line, std::vector<VirtualServer*>& virtualServers);

	static std::string		server_current_fields[virtual_server_directive];
	static std::string		location_current_fields[location_directive];

	class	ParseConfigFileException: public std::exception {
	private:
		std::string		_message;
	public:
		ParseConfigFileException(std::string const &message) : _message("Config File: " + message) { }
		virtual ~ParseConfigFileException() throw() { }

		virtual const char*		what() const throw() { return (_message.c_str()); }
	};
};

#endif //WEBSERV_PARSECONFIGFILE_HPP
