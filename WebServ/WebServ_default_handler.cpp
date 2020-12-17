/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_default_handler.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 23:04:25 by imicah            #+#    #+#             */
/*   Updated: 2020/12/17 11:15:27 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

std::string	WebServ::_AutoindexGenerate(Request *request, const std::string& path_to_target) {
	std::string 		body_response;
	DIR*				directory;
	dirent*				current_file;

	directory = opendir(path_to_target.c_str());
	current_file = readdir(directory);
	current_file = readdir(directory);
	body_response.append("<html><head><title>Index of " + request->GetTarget() + "</title></head><body>"
								 "<h1>Index of " + request->GetTarget() + "</h1><hr><pre><a href=\"../\">../</a><br>");
	while ((current_file = readdir(directory)) != nullptr) {
		std::string		file(current_file->d_name);
		if (current_file->d_type == 4)
			file.append("/");
		body_response.append("<a href=\"" + file + "\">" + file + "</a><br>");
	}
	body_response.append("</pre><hr></body></html>");
	return (body_response);
}
