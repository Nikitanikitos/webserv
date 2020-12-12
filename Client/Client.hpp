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
	std::string		_buffer;
	int 			_stage;
	Request*		_request;
	Response*		_response;
	bool 			_in_proccessed;

	pthread_mutex_t*		_stage_mutex;
	pthread_mutex_t*		_proccess_mutex;

public:
	Client(int, int);
	virtual ~Client();

	const std::string&			get_buffer() const;
	Request*					get_request() const;
	Response*					get_response() const;
	int							get_stage() const;
	int							get_socket() const;

	void						set_processed(bool processed);
	void						set_request(Request* request);
	void						set_response(Response* response);
	void						set_stage(int stage);

	void						add_to_buffer(char *);

	void						next_stage();

	bool						ready_to_action(fd_set* set, int stage) const;

	void						lock_stage_mutex();
	void						unlock_stage_mutex();

	bool						in_task_queue();
};

#endif //WEBSERV_CLIENT_HPP
