/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 16:04:10 by imicah            #+#    #+#             */
/*   Updated: 2020/12/06 02:10:36 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSEEXCEPTION_HPP
# define WEBSERV_RESPONSEEXCEPTION_HPP

# include <exception>
# include <string>
# include <fstream>
#include <Response.hpp>
# include "libft.hpp"

class	ResponseException : public std::exception, public Response {
private:
	std::string		_error_page;

public:
	ResponseException(const std::string& status_code, const std::string& message_phrase, const std::string &error_page);
	~ResponseException() override = default;

	virtual void	generate_response(int client_socket);
	virtual void	clear();
};

class	Request301Redirect : public std::exception, public Response  {
private:
	const std::string	_location;

public:
	explicit Request301Redirect(const std::string& location);

	virtual ~Request301Redirect() = default;

	virtual void			generate_response(int) const;
};

#endif //WEBSERV_RESPONSEEXCEPTION_HPP
