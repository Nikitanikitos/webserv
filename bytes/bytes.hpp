/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytes.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 15:08:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/20 15:08:50 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_BYTES_HPP
# define WEBSERV_BYTES_HPP

# include <string>

struct		bytes {
private:
	size_t			_size;
	char*			_buffer;

	char*			_bytedup(const char* src, int size);

public:
	bytes() : _size(0), _buffer() { }
	~bytes() { delete []_buffer; }
	bytes(const bytes& string) : _size(string._size) { _buffer = _bytedup(string._buffer, _size); }
	bytes(const std::string& string) : _size(string.size()) { _buffer = _bytedup(string.c_str(), string.size()); }

	bytes&			operator=(const bytes& string);

	void			add(const std::string& string);
	void			add(const char* string, int i);
	void			add(const bytes& string);
	void			erase(size_t pos = 0, size_t n = -1);

	size_t			size() const;
	void			clear();
	const char*		c_str() const;
};

#endif //WEBSERV_BYTES_HPP
