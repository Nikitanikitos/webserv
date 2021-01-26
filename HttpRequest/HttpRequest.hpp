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
# include "libft.hpp"
# include <algorithm>

enum stage {
	parsing_first_line,
	parsing_headers,
	parsing_body,
	completed,
};

class	HttpRequest : public HttpObject {
private:
	static const std::string	methods[4];

	std::string		method;
	std::string		query;
	std::string		target;
	int				stage;
	int 			chunk_size;

	void								parsingFirstLine(std::string line_request);
	void								parsingBodyByContentLength();
	void								parsingBodyByChunked();
	bool 								isValidMethod(const std::string& method_);
	void								parseHeader(const std::string& line);
	void								endOfHeaders();

public:
	HttpRequest() : stage(0), chunk_size(-1) { }
	virtual ~HttpRequest() { }

	inline void							setTarget(const std::string& target_) {
		target = target_.substr(0, target_.find('?'));
		query = (target_.find('?') != std::string::npos) ? target_.substr(target_.find('?') + 1) : "";
	}

	inline void							setMethod(const std::string& method_) { method = method_; }
	inline void							setStage(int stage_) { stage = stage_; }

	inline const std::string&			getMethod() const { return (method); }
	inline const std::string&			getTarget() const { return (target); }
	inline int							getStage() const { return (stage); }
	inline int							getChunkSize() const { return (chunk_size); }
	inline const std::string&			getQuery() const { return (query); }

	void								addDataToRequest(char* data, size_t size);

	virtual void						clear();

	bytes								getRequestData();
	inline void							trimBody(size_t n) { body.rtrim(n); }
};

#endif //WEBSERV_HTTPREQUEST_HPP
