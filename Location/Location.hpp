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
# include <map>
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

class	Location {
private:
	std::map<std::string, std::string>			cgi;
	std::vector<bool>							allow_methods;
	std::string									path;
	std::string									root;
	std::string									index;
	bool										autoindex;
	size_t										limit_client_body_size;
//												client_max_body_size // TODO переименовать?

public:
	Location() : autoindex(non_accepted), limit_client_body_size(-1)
		{ allow_methods.assign(count_methods, accepted); }
	~Location() { }

	inline void						eraseAcceptedMethods() { allow_methods.assign(count_methods, non_accepted); }
	inline void						addAllowMethod(int method) { allow_methods[method] = accepted; }

	inline void						setRoot(const std::string& root_) { root = root_; }
	inline void						setIndex(const std::string& index_) { index = index_; }
	inline void						addCgi(const std::string& extension, const std::string path_)
		{ cgi.insert(std::make_pair(extension, path_)); }
	inline void						setAutoindex(bool autoindex_) { autoindex = autoindex_; }
	inline void						setPath(const std::string& path_) { path = path_; }
	inline void						setLimitClientBodySize(size_t limitClientBodySize)
		{ limit_client_body_size = limitClientBodySize; }

	inline const std::string&		getIndex() const { return (index); }
	inline size_t					getLimitClientBodySize() const { return (limit_client_body_size); }
	inline const std::string&		getPath() const { return (path); }
	inline const std::string&		getRoot() const { return (root); }
	inline const std::string&		getCgiInterpreter(const std::string& key) { return cgi.at(key); }
	inline bool						getAutoindex() const { return (autoindex); }

	bool							isAllowMethod(const std::string& method) const;
	bool							findCgi(const std::string& file) const;
};

#endif //WEBSERV_LOCATION_HPP
