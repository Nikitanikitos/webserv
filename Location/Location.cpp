/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:30:05 by nikita            #+#    #+#             */
/*   Updated: 2020/12/21 12:37:06 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : autoindex(non_accepted), location_type(default_location)
	{ allow_methods.assign(5, accepted); }

void	Location::addAllowMethod(bool method) { allow_methods[method] = accepted; }
void	Location::setRoot(const std::string& root_) { root = root_; }
void	Location::setAutoindex(bool autoindex_) { autoindex = autoindex_; }
void	Location::setCgiPath(const std::string& cgi_path) { cgi_pass = cgi_path; }
void	Location::setIndex(const std::string& index_) { index = index_; }
void	Location::setLocationType(bool location_type_) { location_type = location_type_; }
void	Location::setExtension(const std::string &extension_) { extension = extension_;}
void	Location::setPath(const std::string& path_) { path = path_; }

void	Location::eraseAcceptedMethods() {
	for (int i = 0; i < allow_methods.size(); ++i)
		allow_methods[i] = false;
}

const std::vector<bool>&	Location::getAllowMethods() const { return (allow_methods); }
const std::string&			Location::getPath() const { return (path); }
const std::string&			Location::getRoot() const { return (root); }
const std::string&			Location::getExtension() const { return (extension); }
const std::string&			Location::getIndex() const { return (index); }
bool						Location::getLocationType() const { return (location_type); }
bool						Location::getAutoindex() const { return (autoindex); }

bool						Location::isAllowMethod(const std::string& method) const {
	static std::string	methods[NUMBER_METHODS] = {"GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS"};

	for (int i = 0; i < NUMBER_METHODS; ++i) {
		if (method == methods[i])
			return (allow_methods[i]);
	}
	return (false);
}
