/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 18:14:03 by imicah            #+#    #+#             */
/*   Updated: 2020/12/16 18:56:00 by imicah           ###   ########.fr       */
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
	explicit Response(const std::string& status_code, const std::string& message_phrase);
	virtual ~Response();

	const std::string&		GetHeader(const std::string& key) const;
	const std::string&		GetBuffer() const;

	const std::string& GetStatusCode() const;

	void					AddHeader(const std::string& key, const std::string& value);
	void					SetStatusCode(const std::string& status_code);
	void					SetBody(const std::string& body);

	void					GenerateResponse();
	int						SendResponse(int client_socket);

	void					Clear();
};

#endif //WEBSERV_RESPONSE_HPP
