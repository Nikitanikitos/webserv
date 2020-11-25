/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 18:14:03 by imicah            #+#    #+#             */
/*   Updated: 2020/11/25 02:03:43 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
# define WEBSERV_RESPONSE_HPP

# include <string>
# include <map>
# include <ctime>

class Response {
private:
	std::string								_status_code;
	std::map<std::string, std::string>		_headers;
	std::string 							_body;
	std::string 							_response;
	int 									_body_size;

public:
	Response();
	virtual ~Response();

	void add_header(const std::pair<std::string, std::string>&);
	void set_status_code(const std::string&);
};


#endif //WEBSERV_RESPONSE_HPP
