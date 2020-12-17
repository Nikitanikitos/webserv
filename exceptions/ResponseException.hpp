/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseException.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 16:04:10 by imicah            #+#    #+#             */
/*   Updated: 2020/12/16 15:22:25 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSEEXCEPTION_HPP
# define WEBSERV_RESPONSEEXCEPTION_HPP

# include <exception>
# include <string>
# include <fstream>
#include <Response.hpp>
# include "libft.hpp"

class	ResponseException : public Response {
private:
	std::string		_error_page;

public:
	ResponseException(const std::string& status_code, const std::string& message_phrase, const std::string &error_page);
	virtual ~ResponseException() = default;

//	virtual void	GenerateResponse();
//	virtual void	Clear();
};

#endif //WEBSERV_RESPONSEEXCEPTION_HPP
