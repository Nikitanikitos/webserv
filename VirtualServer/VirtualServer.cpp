//
// Created by nikita on 21.11.2020.
//

#include "VirtualServer.hpp"

VirtualServer::VirtualServer() { }

void VirtualServer::set_host(const std::string& host) {
	_host = host;
}

void VirtualServer::add_port(const std::string& port) {
	_ports.push_back(port);
}

void VirtualServer::add_server_name(const std::string& server_name) {
	_server_name.push_back(server_name);
}

void VirtualServer::add_route(const Route& route) {
	_list_routers.push_back(route);
}

void VirtualServer::initialize_sockets() {

}

VirtualServer::~VirtualServer() {

}

void VirtualServer::set_limit_client_body_size(int limit_client_body_size) {
	_limit_client_body_size = limit_client_body_size;
}
