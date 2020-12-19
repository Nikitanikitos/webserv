/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 11:41:09 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 15:25:02 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bytes.hpp"

const bytes&	bytes::append(const std::string& string) {
	append(string.c_str(), string.size());
	return (*this);
}

const char*		bytes::c_str() const { return (_buffer); }

const bytes&	bytes::append(const char* string, int i) {
	if (*_buffer == 0) {
		free(_buffer);
		_buffer = _bytedup(string, i);
		_size = i;
	}
	else {
		char*	temp_buff;

		temp_buff = _buffer;
		_buffer = (char*)malloc(_size + i);
		for (int j = 0; j < _size ; ++j)
			_buffer[j] = temp_buff[j];
		for (int k = 0; k < i; ++k)
			_buffer[_size++] = string[k];
		free(temp_buff);
	}
	return (*this);
}

const bytes&	bytes::append(const bytes& string) {
	append(string.c_str(), string.size());
	return (*this);
}

size_t			bytes::size() const { return (_size); }

void	bytes::clear() {
	free(_buffer);
	_size = 0;
	_buffer = 0;
}

const bytes&	bytes::erase(size_t pos, size_t n) {
	if (n >= _size) {
		free(_buffer);
		_buffer = 0;
		_size = 0;
	}
	return (*this);
}

bytes&		bytes::operator=(const bytes& string) {
	free(_buffer);
	_buffer = _bytedup(string._buffer, string._size);
	_size = string._size;
	return (*this);
}

char* bytes::_bytedup(const char* src, int size) {
	char	*result;
	size_t	i;

	i = 0;
	if (!(result = (char*)malloc(sizeof(char) * size)))
		return (0);
	for (; i < size; ++i)
		result[i] = src[i];
	return (result);
}
