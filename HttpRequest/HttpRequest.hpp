/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 18:03:37 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 19:17:53 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HTTPREQUEST_HPP
# define WEBSERV_HTTPREQUEST_HPP

# include "HttpObject.hpp"

enum stage {
	parsing_first_line,
	parsing_headers,
	parsing_body,
	completed,
	bad_request,
};

class	HttpRequest : public HttpObject {
private:
	std::string				method;
	std::string				target;
	int						stage;

public:
	HttpRequest() { }
	virtual ~HttpRequest() { }

	void							setTarget(const std::string& target_);
	void							setMethod(const std::string& method_);
	void							setStage(int stage);

	const std::string&				getMethod() const;
	const std::string&				getTarget() const;
	int								getStage() const;

	virtual void					clear();

	bytes							getRequestLine();
};

#endif //WEBSERV_HTTPREQUEST_HPP
