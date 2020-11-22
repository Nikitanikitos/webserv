/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 18:03:37 by imicah            #+#    #+#             */
/*   Updated: 2020/11/22 15:25:48 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUEST_HPP
# define WEBSERV_REQUEST_HPP

# include <string>
# include <map>

class	Request
{
private:
	typedef		std::map<std::string, std::string>		_headline_struct;

	std::string				_method;
	std::string				_target;
	std::string				_host;
	std::string				_port;
	_headline_struct		_headlines;

public:
	Request();

	void set_host(const std::string& host);

	void set_target(const std::string& target);

	void set_method(const std::string& method);

};

#endif //WEBSERV_REQUEST_HPP
