/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:29:23 by nikita            #+#    #+#             */
/*   Updated: 2020/11/26 12:39:53 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_LOCATION_HPP
# define WEBSERV_LOCATION_HPP

# define NUMBER_METHODS		6

# include <vector>
# include <string>
# include <cstdint>

enum {
	accepted = true,
	non_accepted = false
};

enum {
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	OPTIONS,
};

enum {
	default_location = true,
	cgi_location = false,
};

class	Location {
private:
	std::vector<bool>	_allow_methods;
	std::string			_extension;
	std::string			_path;
	std::string			_root;
	std::string			_cgi_pass;
	std::string			_index;
	bool				_autoindex;
	bool 				_location_type;

public:
	Location();
	~Location() { }

	void						AddAllowMethod(bool method);
	void 						EraseAcceptedMethods();
	bool						IsAllowMethod(const std::string& method) const;

	void						SetLocationType(bool location_type);
	void						SetRoot(const std::string& root);
	void						SetIndex(const std::string& index);
	void						SetCgiPath(const std::string& cgi_path);
	void						SetAutoindex(bool autoindex);
	void						SetPath(const std::string& path);
	void 						SetExtension(const std::string &extension);

	const std::string&			GetIndex() const;
	const std::string&			GetPath() const;
	const std::string&			GetExtension() const;
	const std::string&			GetRoot() const;
	const std::vector<bool>&	GetAllowMethods() const;
	bool						GetLocationType() const;
	bool						GetAutoindex() const;

};

#endif //WEBSERV_LOCATION_HPP
