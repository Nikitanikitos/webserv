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
# define WEBSERV_CLIENT_HPP

# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include <zconf.h>
# include <arpa/inet.h>

enum Stage {
	parsing_request,
	generate_response,
	send_response,
	close_connection,
};

enum TimeOut { TimeOut = 120, };

class	Client {
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

	inline const std::string&		getPort() const { return (port); }
	inline const std::string&		getHost() const  { return (host); }
	inline int						getStage() const { return (stage); }
	inline int						getSocket() const { return (socket); }
	inline HttpRequest*				getRequest() const { return (request); }
	inline HttpResponse*			getResponse() const { return (response); }

	inline void						setProcessed(bool processed) { in_proccessed = processed; }
	inline void						setStage(int stage_) { stage = stage_; }

	inline bool						inTaskQueue() { return (in_proccessed); }

	bool							connectionTimedOut();
	void							setNewConnectionTime();

	inline void						clearResponse() { response->clear(); }
	inline void						clearRequest() { request->clear(); }

	inline void						sendResponse() { response->sendResponse(socket); }
	inline void 					generateResponse() { response->generateResponse(); }
};

#endif //WEBSERV_CLIENT_HPP
