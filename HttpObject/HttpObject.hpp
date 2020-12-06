/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpObject.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 15:36:35 by imicah            #+#    #+#             */
/*   Updated: 2020/12/06 01:12:41 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HTTPOBJECT_HPP
#define WEBSERV_HTTPOBJECT_HPP

# include "Request.hpp"
# include "Response.hpp"

enum stage {
	generate_request_,
	generate_response_,
	send_response_,
	complited_
};

class HttpObject {
private:
	int 		_client_socket;
	int 		_stage;
	Request		_request;
	Response	_response;

public:
	HttpObject(int, int);
	virtual ~HttpObject();

	[[nodiscard]] const Request&	get_request() const;
	[[nodiscard]] const Response&	get_response() const;

	[[nodiscard]] int				get_stage() const;
	[[nodiscard]] int				get_client_socket() const;

	void							set_request(const Request& request);
	void							set_response(const Response& response);

	void							next_stage();
};


#endif //WEBSERV_HTTPOBJECT_HPP
