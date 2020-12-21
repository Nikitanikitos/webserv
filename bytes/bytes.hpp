/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytes.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 15:08:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 13:58:12 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_BYTES_HPP
# define WEBSERV_BYTES_HPP

# include <string>

struct		bytes {
private:
	size_t			size_;
	char*			buffer;

	char*			bytedup(const char* src, int size);

public:
	bytes() : size_(0), buffer() { }
	~bytes() { delete []buffer; }

	bytes(const bytes& string) : size_(string.size_) { buffer = bytedup(string.buffer, size_); }
	bytes(const std::string& string) : size_(string.size()) { buffer = bytedup(string.c_str(), string.size()); }

	bytes&			operator=(const bytes& string);

	void			add(const std::string& string);
	void			add(const char* string, int i);
	void			add(const bytes& string);
	void			erase(size_t pos = 0, size_t n = -1);

	size_t			size() const;
	void			clear();
	const char*		c_str() const;

	size_t			find(const char* needle);
	bytes			substr(size_t i);
};

#endif //WEBSERV_BYTES_HPP
