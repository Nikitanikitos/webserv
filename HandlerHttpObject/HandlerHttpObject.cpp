#include "HandlerHttpObject.hpp"

const std::string		HandlerHttpObject::methods[4] =
	{"GET", "HEAD", "POST", "PUT"};

static void				setBadRequest(HttpRequest* request, HttpResponse* response, const std::string& status_code) {
	request->setStage(bad_request);
	response->setStatusCode(status_code);
}

void HandlerHttpObject::parsingFirstLine(HttpRequest* request, HttpResponse* response, std::string line_request) {
	if (std::count(line_request.begin(), line_request.end(), ' ') != 2) {
		setBadRequest(request, response, "400");
		return;
	}

	std::string element;
	for (int i = 0; i < 3; ++i) {
		element = line_request.substr(0, line_request.find(' '));
		if (i == 0) {
			if (!isValidMethod(element)) {
				setBadRequest(request, response, "400");
				return;
			}
			else request->setMethod(element);
		}
		else if (i == 1)
			request->setTarget(element);
		else if (element != "HTTP/1.1" && element != "HTTP/1.0") {
			setBadRequest(request, response, "400");
			return;
		}
		line_request.erase(0, line_request.find(' ') + 1);
	}
	request->setStage(parsing_headers);
}

bool	HandlerHttpObject::isValidMethod(const std::string& method) {
	for (size_t i = 0; i < 4; ++i)
		if (methods[i] == method) return (true);
	return (false);
}

bool	HandlerHttpObject::parseHeader(HttpRequest* request, const std::string& line) {
	size_t		position;
	std::string key;
	std::string value;

	if (std::count(line.begin(), line.end(), ':') < 1)
		return (false);
	position = line.find(':');
	key = line.substr(0, position);
	position += (line[position + 1] == ' ') ? 2 : 1;
	value = line.substr(position);
	std::for_each(key.begin(), key.end(), ft_tolower);
	request->addHeader(key, value);
	return (true);
}

bool	HandlerHttpObject::isErrorStatus(const std::string& status) {
	const std::string	status_code[6] = {"400", "403", "404", "405", "411", "413"};

	for (int i = 0; i < status_code->size(); ++i)
		if (status == status_code[i]) return (true);
	return (false);
}

void	HandlerHttpObject::endOfHeaders(HttpRequest* request, HttpResponse* response) {
	if (!request->findHeader("host")) {
		response->setStatusCode("400");
		request->setStage(bad_request);
	}
	else if (request->getMethod() == "PUT" || request->getMethod() == "POST") {
		if (!request->findHeader("content-length")) {
			response->setStatusCode("411");
			request->setStage(bad_request);
		}
//		else if (ft_atoi(request->getHeader("content-length").c_str()) > limit_client_body_size) {
//			response->setStatusCode("413");
//			request->setStage(bad_request);
//		}
		else
			request->setStage(parsing_body);
	}
	else
		request->setStage(completed);
}
