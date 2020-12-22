/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 15:36:35 by imicah            #+#    #+#             */
/*   Updated: 2020/12/06 01:12:41 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include <zconf.h>

enum Stage {
	read_request_,
	parsing_request_,
	generate_response_,
	send_response_,
	close_connection_,
};

enum TimeOut { TimeOut = 10, };

class Client {
private:
	int 					socket;
	int 					stage;
	bool 					in_proccessed;
	std::string				host;
	std::string				port;
	HttpRequest*			request;
	HttpResponse*			response;
	long					connection_time;

public:
	Client(int socket, const std::string& ip, const std::string& port);
	virtual ~Client();

	const std::string&			getPort() const;
	const std::string&			getHost() const;
	int							getStage() const;
	int							getSocket() const;
	HttpRequest*				getRequest() const;
	HttpResponse*				getResponse() const;

	void						setProcessed(bool processed);

	void						setStage(int stage_);
	void						nextStage();

	bool						inTaskQueue();

	bool						connectionTimedOut();
	void						setNewConnectionTime();

	void						clearResponse();
	void						clearRequest();

	void						sendResponse();
	void 						generateResponse();

	bytes						getRequestLine();
};

#endif //WEBSERV_CLIENT_HPP
