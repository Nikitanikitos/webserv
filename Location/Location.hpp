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

# include <vector>
# include <string>

enum {
	accepted = true,
	non_accepted = false
};

enum { count_methods = 4, };

enum {
	GET,
	HEAD,
	POST,
	PUT,
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
	size_t				limit_client_body_size;
//	                    client_max_body_size // TODO переименовать?

public:
	Location();
	~Location() { }

	inline void						eraseAcceptedMethods() { allow_methods.assign(count_methods, non_accepted); }
	inline void						addAllowMethod(int method) { allow_methods[method] = accepted; }
	bool							isAllowMethod(const std::string& method) const;

	inline void						setLocationType(bool location_type_) { location_type = location_type_; }
	inline void						setRoot(const std::string& root_) { root = root_; }
	inline void						setIndex(const std::string& index_) { index = index_; }
	inline void						setCgiPath(const std::string& cgi_path) { cgi_pass = cgi_path; }
	inline void						setAutoindex(bool autoindex_) { autoindex = autoindex_; }
	inline void						setPath(const std::string& path_) { path = path_; }
	inline void 					setExtension(const std::string &extension_) { extension = extension_;}
	inline void						setLimitClientBodySize(size_t limitClientBodySize)
		{ limit_client_body_size = limitClientBodySize; }

	inline const std::string&		getIndex() const { return (index); }
	inline size_t					getLimitClientBodySize() const { return (limit_client_body_size); }
	inline const std::string&		getPath() const { return (path); }
	inline const std::string&		getExtension() const { return (extension); }
	inline const std::string&		getRoot() const { return (root); }
	inline bool						getLocationType() const { return (location_type); }
	inline bool						getAutoindex() const { return (autoindex); }
};

#endif //WEBSERV_LOCATION_HPP
