/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:50:51 by imicah            #+#    #+#             */
/*   Updated: 2020/11/25 04:21:46 by imicah           ###   ########.fr       */
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

	VirtualServer				_parse_vs_directive();

	Location					_parse_location_directive(std::string const &);

	std::vector<std::string>	_getArgsFromLine(std::string &input) const;

	int 						_getIndexOfArg(std::string const &arg, std::string *arr, int size) const;

	bool 						_checkTabulation(std::string const &line, int expectedTabCount) const;

	void 						_add_allow_methods_to_location(Location &location, std::vector<std::string> const& trimmedStr);

	void 						_set_autoindex_in_location(Location &location, std::vector<std::string> const& trimmedStr);

public:
	explicit ParseConfigFile(char *filename);
	~ParseConfigFile() = default;

	std::vector<VirtualServer> parse_file(std::string &number_of_workers);
	/* метод будет возвращать список виртуальных серверов, в которых есть список роутеров */
	class ParseConfigFileException: public std::exception {
	private:
		std::string 			_message;
	public:
		ParseConfigFileException(std::string const &message);
		virtual const char* what() const throw();
	};
	static std::string serverCurrentFields[6];
	static std::string locationCurrentFields[7];
};

#endif //WEBSERV_PARSECONFIGFILE_HPP
