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

enum stage {
	read_request_,
	parsing_request_,
	generate_response_,
	send_response_,
	completed_,
	close_connection_,
};

class Client {
private:
	int 			_socket;
	std::string		_ip;
	std::string		_port;
	std::string		_buffer;
	int 			_stage;
	Request			_request;
	Response		_response;
	bool 			_in_proccessed;

	pthread_mutex_t*		_proccess_mutex;

public:
	Client(int socket, int stage, const std::string& ip, const std::string& port);
	virtual ~Client();

	const std::string&			GetBuffer() const;
	const std::string&			GetPort() const;
	const std::string&			GetIp() const;
	Request&					GetRequest();
	Response&					GetResponse();
	int							GetStage() const;
	int							GetSocket() const;

	void						SetProcessed(bool processed);
	void						SetRequest(const Request &request);
	void						SetResponse(const Response &response);
	void						SetStage(int stage);

	void						AddToBuffer(char *data);

	void						NextStage();

	bool						InTaskQueue();

	void						ClearBuffer();
	void						ClearResponse();
	void						ClearRequest();
};

#endif //WEBSERV_CLIENT_HPP
