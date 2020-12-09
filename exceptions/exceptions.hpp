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

#ifndef WEBSERV_EXCEPTIONS_HPP
# define WEBSERV_EXCEPTIONS_HPP

# include <exception>
# include <string>
# include <fstream>
#include <Response.hpp>
# include "libft.hpp"

class	RequestException : public std::exception, public Response {
private:
	const std::string	_message_phrase;
	const std::string	_status_code;
	const std::string	_error_page;

public:
	RequestException(const std::string& status_code, const std::string& message_phrase, const std::string &error_page);
	~RequestException() override = default;

	void	generate_response(int client_socket) const override;
};

class	Request301Redirect : public std::exception, public Response  {
private:
	const std::string	_message_phrase;
	const std::string	_status_code;
	const std::string	_location;

public:
	explicit Request301Redirect(const std::string& location);

	virtual ~Request301Redirect() = default;

	void				generate_response(int) const override ;
};

#endif //WEBSERV_EXCEPTIONS_HPP
