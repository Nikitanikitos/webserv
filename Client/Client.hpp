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

# include "Request.hpp"
# include "Response.hpp"

enum Stage {
	read_request_,
	parsing_request_,
	generate_response_,
	send_response_,
	close_connection_,
};

enum TimeOut {
	TimeOut = 3,
};

class Client {
private:
	int 					_socket;
	int 					_stage;
	bool 					_in_proccessed;
	std::string				_ip;
	std::string				_port;
	Request*				_request;
	Response*				_response;
	long					_connection_time;

public:
	Client(int socket, const std::string& ip, const std::string& port);
	virtual ~Client();

	const std::string&			GetPort() const;
	const std::string&			GetIp() const;
	int							GetStage() const;
	int							GetSocket() const;
	Request						*GetRequest() const;
	Response					*GetResponse() const;

	void SetNewConnectionTime();

	void						SetProcessed(bool processed);

	void						SetStage(int stage);
	void						NextStage();

	bool						InTaskQueue();
	bool						ConnectionTimedOut();

	void						ClearResponse();
	void						ClearRequest();

	void						SendResponse();
	void 						GenerateResponse();
};

#endif //WEBSERV_CLIENT_HPP
