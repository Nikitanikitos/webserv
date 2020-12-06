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

/* lvs - list virtual servers */

class	ParseConfigFile {
private:
	char			*_filename;
	int 			_fd;

	VirtualServer	_parse_vs_directive();
	/* Метод будет возвращать объект класса VirtualServer со всеми инициализированными полями */

	Location			_parse_location_directive();
	/* Метод будет возвращать объект класса Route со всеми инициализированными полями */

public:
	explicit ParseConfigFile(char *filename);
	~ParseConfigFile() = default;

	std::vector<VirtualServer>		parse_file();
	/* метод будет возвращать список виртуальных серверов, в которых есть список роутеров */
};

#endif //WEBSERV_PARSECONFIGFILE_HPP
