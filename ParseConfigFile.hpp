/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:50:51 by imicah            #+#    #+#             */
/*   Updated: 2020/11/22 14:20:45 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_PARSECONFIGFILE_HPP
# define WEBSERV_PARSECONFIGFILE_HPP

# include <string>
# include "VirtualServer.hpp"
# include "Route.hpp"

/* lvs - list virtual servers */

class	ParseConfigFile
{
private:
	std::string		_filename;

	VirtualServer	_parse_vs_directive();
	/* Метод будет возвращать объект класса VirtualServer со всеми инициализированными полями */

	Route			_parse_route_directive();
	/* Метод будет возвращать объект класса Route со всеми инициализированными полями */

public:
	explicit ParseConfigFile(std::string& filename);
	~ParseConfigFile() = default;

	std::vector<VirtualServer>		parse_file();
	/* метод будет возвращать список виртуальных серверов, в которых есть список роутеров */
};

#endif //WEBSERV_PARSECONFIGFILE_HPP
