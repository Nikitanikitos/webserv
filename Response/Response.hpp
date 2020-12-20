/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 18:14:03 by imicah            #+#    #+#             */
/*   Updated: 2020/12/19 15:16:18 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
# define WEBSERV_RESPONSE_HPP

# include <string>
# include <map>
# include <ctime>
# include "libft.hpp"
# include <fcntl.h>
# include <iostream>

class Response {
protected:
	std::string								_status_code;
	std::map<std::string, std::string>		_headers;
	bytes 									_body;
	bytes	 								_buffer;
	std::string								_message_phrase;

public:
	static const std::map<std::string, std::string>	_message_phrases;

	Response() { }
	~Response() { }

	const std::string&		GetHeader(const std::string& key) const;
	const bytes&			GetBuffer() const;
	const std::string&		GetStatusCode() const;

	void					AddHeader(const std::string& key, const std::string& value);
	void					SetStatusCode(const std::string& status_code);
	void					SetBody(const bytes& body);

	void					GenerateResponse();
	int						SendResponse(int client_socket);

	void					Clear();
};

#endif //WEBSERV_RESPONSE_HPP
