//
// Created by nikita on 21.11.2020.
//

#include "WebServ.hpp"

WebServ::WebServ(const WebServ::lvs_type &listVirtualServers) : list_virtual_servers(listVirtualServers) { }

WebServ::~WebServ() {

}

std::map<std::string, std::string> WebServ::_check_request_header() {
	return std::map<std::string, std::string>();
}

void WebServ::_execute_cgi_client() {

}

void WebServ::_parse_request_body() {

}

void WebServ::_get_request(VirtualServer&) {

}

void WebServ::_give_response() {

}

void WebServ::run_server() {

}

void WebServ::serve_client(VirtualServer&) {

}
