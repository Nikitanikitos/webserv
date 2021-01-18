/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytes.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 15:08:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 17:10:07 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_BYTES_HPP
# define WEBSERV_BYTES_HPP

# include <string>

struct		bytes {
private:
	size_t			size_;
	char*			buffer;

	inline char*	bytedup(const bytes& src, size_t size) { return (bytedup(src.buffer, size)); }
	char*			bytedup(const char* src, size_t size);

public:
	bytes() : size_(0), buffer() { buffer = new char[1]; }
	~bytes() { delete []buffer; }

	bytes(const char* data, size_t size) : size_(size) { buffer = bytedup(data, size); }
	bytes(const bytes& string) : size_(string.size_) { buffer = bytedup(string.buffer, size_); }
	bytes(const std::string& string) : size_(string.size()) { buffer = bytedup(string.c_str(), string.size()); }

	bytes&			operator=(const bytes& string);

	inline void		add(const std::string& string) { add(string.c_str(), string.size()); }
	void			add(const char* string, size_t i);
	inline void		add(const bytes& string) { add(string.c_str(), string.size()); }
	void			erase(size_t n);
	void			rtrim(size_t n);

	void			clear();
	size_t			find(const char* needle);
	size_t			rfind(const char* needle);

	bytes			substr(size_t i);

	inline size_t			size() const { return (size_); }
	inline bool				empty() const { return (!size_); }
	inline const char*		c_str() const { return (buffer); }
};

#endif //WEBSERV_BYTES_HPP
