//
// Created by nikita on 22.12.2020.
//

#ifndef WEBSERV_HANDLERHTTPOBJECT_HPP
# define WEBSERV_HANDLERHTTPOBJECT_HPP

# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include <algorithm>
# include "libft.hpp"

class HandlerHttpObject {
public:
	static const std::string		methods[4];

	static void				parsingFirstLine(HttpRequest* request, HttpResponse* response, std::string line_request);
	static bool 			parseHeader(HttpRequest *request, const std::string& line);

	static bool 			isValidMethod(const std::string& method);
	static bool				isErrorStatus(const std::string& status);

	static void				endOfHeaders(HttpRequest* request, HttpResponse* response);
};

#endif //WEBSERV_HANDLERHTTPOBJECT_HPP
