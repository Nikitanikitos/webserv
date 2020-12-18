/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:30:05 by nikita            #+#    #+#             */
/*   Updated: 2020/12/17 11:12:16 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : _autoindex(non_accepted), _location_type(default_location) {
	_allow_methods.assign(5, accepted);
}

void	Location::AddAllowMethod(bool method) { _allow_methods[method] = accepted; }
void	Location::SetRoot(const std::string& root) { _root = root; }
void	Location::SetAutoindex(bool autoindex) { _autoindex = autoindex; }
void	Location::SetCgiPath(const std::string& cgi_path) { _cgi_pass = cgi_path; }
void	Location::SetIndex(const std::string& index) { _index = index; }
void	Location::SetLocationType(bool location_type) { _location_type = location_type; }
void	Location::SetExtension(const std::string &extension) { _extension = extension;}
void	Location::SetPath(const std::string& path) { _path = path; }

void	Location::EraseAcceptedMethods() {
	for (int i = 0; i < _allow_methods.size(); ++i)
		_allow_methods[i] = false;
}

const std::vector<bool>&	Location::GetAllowMethods() const { return (_allow_methods); }
const std::string&			Location::GetPath() const { return (_path); }
const std::string&			Location::GetRoot() const { return (_root); }
const std::string&			Location::GetExtension() const { return (_extension); }
const std::string&			Location::GetIndex() const { return (_index); }
bool						Location::GetLocationType() const { return (_location_type); }
bool						Location::GetAutoindex() const { return (_autoindex); }

bool						Location::IsAllowMethod(const std::string& method) const {
	static std::string	methods[NUMBER_METHODS] = {"GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS"};

	for (int i = 0; i < NUMBER_METHODS; ++i) {
		if (method == methods[i])
			return (_allow_methods[i]);
	}
	return (false);
}
