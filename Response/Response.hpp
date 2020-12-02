/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 18:14:03 by imicah            #+#    #+#             */
/*   Updated: 2020/12/02 16:13:01 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
# define WEBSERV_RESPONSE_HPP

# include <string>
# include <map>
# include <ctime>
# include "libft.hpp"

class Response {
private:
	std::string								_status_code;
	std::map<std::string, std::string>		_headers;
	std::string 							_body;
	std::string 							_response;
	std::string								_message_phrase;

public:

	Response();
	explicit Response(std::string&);
	virtual ~Response();

	[[nodiscard]] const std::string&		get_header(const std::string& key) const;

	void	add_header(const std::string& key, const std::string& value);
	void	set_status_code(const std::string&);
	void	set_body(const std::string& body);

	void	send_response(int);
};

#endif //WEBSERV_RESPONSE_HPP
