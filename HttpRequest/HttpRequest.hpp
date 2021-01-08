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
};

class	HttpRequest : public HttpObject {
private:
	static const std::string		methods[4];

	std::string				method;
	std::string				query;
	std::string				target;
	int						stage;

	void					parsingFirstLine(std::string line_request);
	void					parsingBodyByContentLength(const bytes& data);
	void					parsingBodyByChunked(bytes& data);
	bool 					isValidMethod(const std::string& method_);
	void					parseHeader(const std::string& line);
	void					endOfHeaders();

public:
	HttpRequest() : stage(0) { }
	virtual ~HttpRequest() { }

	inline void							setTarget(const std::string& target_) { target = target_; }
	inline void							setMethod(const std::string& method_) { method = method_; }
	inline void							setStage(int stage_) {stage = stage_; }

	inline const std::string&			getMethod() const { return (method); }
	inline const std::string&			getTarget() const { return (target); }
	inline int							getStage() const { return (stage); }

	void								addDataToRequest(bytes data);

	virtual void						clear();

	bytes getRequestData(bytes& data);
	inline void							trimBody(size_t n) { body.rtrim(n); }
};

#endif //WEBSERV_HTTPREQUEST_HPP
