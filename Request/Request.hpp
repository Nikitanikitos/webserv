/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 18:03:37 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 11:54:08 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUEST_HPP
# define WEBSERV_REQUEST_HPP

#include <HttpObject.hpp>

class	Request : public HttpObject {
private:
	std::string				_method;
	std::string				_target;

public:
	Request() { }
	virtual ~Request() { }

	void							SetTarget(const std::string& target);
	void							SetMethod(const std::string& method);

	const std::string&				GetMethod() const;
	const std::string&				GetTarget() const;

	virtual void					clear();
};

#endif //WEBSERV_REQUEST_HPP
