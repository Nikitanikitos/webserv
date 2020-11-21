//
// Created by nikita on 21.11.2020.
//

#include "VirtualServer.hpp"

VirtualServer::VirtualServer() { }

void VirtualServer::set_host(in_addr_t host) {
	_host = host;
}

void VirtualServer::add_port(in_port_t port) {
	_ports.push_back(port);
}

void VirtualServer::set_server_name(const std::list<std::string>& server_name) {
	_server_name = server_name;
}

void VirtualServer::add_route(const Route&) {

}

void VirtualServer::initialize_sockets() {

}

VirtualServer::~VirtualServer() {

}
