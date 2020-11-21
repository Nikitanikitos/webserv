/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:50:51 by imicah            #+#    #+#             */
/*   Updated: 2020/11/21 19:49:37 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_PARSECONFIGFILE_HPP
# define WEBSERV_PARSECONFIGFILE_HPP

# include <string>
# include "VirtualServer.hpp"

/* lvs - list virtual servers */

class	ParseConfigFile
{
private:
	typedef		std::map<std::string, VirtualServer>	lvs_type;

	std::string		_filename;

	VirtualServer&	_parse_virtual_server_directive();
	/* Метод будет возвращать объект класса VirtualServer со всеми инициализированными полями */
	Route&			_parse_route_directive();
	/* Метод будет возвращать объект класса Route со всеми инициализированными полями */

public:
	ParseConfigFile(std::string& filename);
	~ParseConfigFile();

	lvs_type		parse_file();
	/* метод будет возвращать список виртуальных серверов, в которых есть список роутеров */
};

#endif //WEBSERV_PARSECONFIGFILE_HPP
