/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 18:14:03 by imicah            #+#    #+#             */
/*   Updated: 2020/12/15 01:59:44 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
# define WEBSERV_RESPONSE_HPP

# include <string>
# include <map>
# include <ctime>
# include "libft.hpp"
# include <fcntl.h>

class Response {
protected:
	std::string								_status_code;
	std::map<std::string, std::string>		_headers;
	std::string 							_body;
	std::string 							_buffer;
	std::string								_message_phrase;

public:
	Response();
	explicit Response(std::string&);
	virtual ~Response();

	const std::string&		get_header(const std::string& key);
	const std::string&		get_buffer();

	void			add_header(const std::string& key, const std::string& value);
	void			set_status_code(const std::string&);
	void			set_body(const std::string& body);

	virtual void generate_response();
	virtual int		send_response(int);

	virtual void	clear();
};

#endif //WEBSERV_RESPONSE_HPP
