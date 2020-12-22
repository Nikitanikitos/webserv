/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 18:03:37 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 22:55:45 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HTTPREQUEST_HPP
# define WEBSERV_HTTPREQUEST_HPP

# include "HttpObject.hpp"
# include <vector>

class	HttpRequest : public HttpObject {
private:
	std::string				method;
	std::string				target;

public:
	HttpRequest() { }
	virtual ~HttpRequest() { }

	void							setTarget(const std::string& target_);
	void							setMethod(const std::string& method_);
	void 							setLocation(const std::string& location);
	void 							setUriAndQueryString(const std::string& location);

	const std::string&				getMethod() const;
	const std::string&				getTarget() const;

	virtual void					clear();
	const char*						getStringBuffer();
};

#endif //WEBSERV_HTTPREQUEST_HPP
