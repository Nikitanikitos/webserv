/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 11:41:09 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 20:34:30 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bytes.hpp"

void			bytes::add(const std::string& string) { add(string.c_str(), string.size()); }
void			bytes::add(const bytes& string) { add(string.c_str(), string.size()); }

const char*		bytes::c_str() const { return (_buffer); }
size_t			bytes::size() const { return (_size); }

void			bytes::add(const char* string, int i) {
	char*	temp_buff;

	if (!_buffer) {
		_buffer = _bytedup(string, i);
		_size = i;
	}
	else {
		temp_buff = _buffer;
		_buffer = new char[_size + i];
		for (int j = 0; j < _size ; ++j)
			_buffer[j] = temp_buff[j];
		for (int k = 0; k < i; ++k)
			_buffer[_size++] = string[k];
		delete []temp_buff;
	}
}

void			bytes::clear() {
	delete []_buffer;
	_size = 0;
	_buffer = 0;
}

void			bytes::erase(size_t pos, size_t n)
	{ if (n >= _size) clear(); }

bytes&		bytes::operator=(const bytes& string) {
	delete []_buffer;
	_buffer = _bytedup(string._buffer, string._size);
	_size = string._size;
	return (*this);
}

char*		bytes::_bytedup(const char* src, int size) {
	char	*result;

	result = new char[size];
	for (int i = 0; i < size; ++i)
		result[i] = src[i];
	return (result);
}
