/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 18:14:03 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:59:06 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HTTPRESPONSE_HPP
# define WEBSERV_HTTPRESPONSE_HPP

# include <sys/time.h>
# include <sys/socket.h>

#include <HttpObject.hpp>
# include "libft.hpp"

# define HTTP_VERSION	(std::string)"HTTP/1.1"
# define SERVER_VERSION	"WebServ/0.1"
# define CRLF			"\r\n"
# define SP				" "

enum { count_status_code = 12, };

class HttpResponse : public HttpObject {
private:
	std::string								status_code;
	std::string								message_phrase;

public:
	static const std::string	message_phrases[count_status_code][2];

	HttpResponse() { }
	virtual ~HttpResponse() { }

	inline const std::string&		getStatusCode() const { return (status_code); }

	inline void						setStatusCode(const std::string& status_code_) { status_code = status_code_; }

	void							generateResponse();
	int								sendResponse(int client_socket);

	virtual void					clear();

	static std::string				getMessagePhrase(const std::string& code);
};

#endif //WEBSERV_HTTPRESPONSE_HPP
