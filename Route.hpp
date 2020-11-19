/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 16:25:22 by imicah            #+#    #+#             */
/*   Updated: 2020/11/18 19:35:14 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_ROUTE_HPP
# define WEBSERV_ROUTE_HPP

# include <set>
# include <string>

class	Route
{
private:
	std::set<std::string>	accepted_methods;
	std::string				root_directory;
	bool					autoindex;

	/* CGI - ?? */
public:
	std::string				get_root_directory();
	bool					get_autoindex_value();
	std::set<std::string>	get_accepted_methods();
};

#endif //WEBSERV_ROUTE_HPP
