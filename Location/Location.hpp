/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:29:23 by nikita            #+#    #+#             */
/*   Updated: 2020/12/21 12:37:06 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_LOCATION_HPP
# define WEBSERV_LOCATION_HPP

# define NUMBER_METHODS		6

# include <vector>
# include <string>

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
	std::vector<bool>	allow_methods;
	std::string			extension;
	std::string			path;
	std::string			root;
	std::string			cgi_pass;
	std::string			index;
	bool				autoindex;
	bool 				location_type;

public:
	Location();
	~Location() { }

	void						addAllowMethod(bool method);
	void 						eraseAcceptedMethods();
	bool						isAllowMethod(const std::string& method) const;

	void						setLocationType(bool location_type_);
	void						setRoot(const std::string& root_);
	void						setIndex(const std::string& index_);
	void						setCgiPath(const std::string& cgi_path);
	void						setAutoindex(bool autoindex_);
	void						setPath(const std::string& path_);
	void 						setExtension(const std::string &extension_);

	const std::string&			getIndex() const;
	const std::string&			getPath() const;
	const std::string&			getExtension() const;
	const std::string&			getRoot() const;
	const std::vector<bool>&	getAllowMethods() const;
	bool						getLocationType() const;
	bool						getAutoindex() const;

};

#endif //WEBSERV_LOCATION_HPP
