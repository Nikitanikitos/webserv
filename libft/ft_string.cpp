/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 11:41:09 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 14:05:39 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.hpp"

const ft::string& ft::string::append(const std::string& string) {
	append(string.c_str(), string.size());
	return (*this);
}

const char* ft::string::c_str() const {
	return (_buffer);
}

const ft::string& ft::string::append(const char* string, int i) {
	if (*_buffer == 0) {
		free(_buffer);
		_buffer = ft_strdup(string);
		_size = ft_strlen(_buffer);
	}
	else {
		char*	temp_buff;

		temp_buff = _buffer;
		_buffer = (char*)malloc(_size + i + 1);
		for (int j = 0; temp_buff[j] ; ++j)
			_buffer[j] = temp_buff[j];
		while (*string)
			_buffer[_size++] = *(string++);
		_buffer[_size] = '\0';
		free(temp_buff);
	}
	return (*this);
}

ft::string& ft::string::append(const string& string) {
	append(string.c_str(), string.size());
	return (*this);
}

size_t ft::string::size() const {
	return (_size);
}

void ft::string::clear() {
	free(_buffer);
	_buffer = 0;
	_size = 0;
	_buffer = 0;
}

const ft::string&	ft::string::erase(size_t pos, size_t n) {
	if (n >= _size) {
		free(_buffer);
		_buffer = 0;
		_size = 0;
	}
	return (*this);
}

ft::string&			ft::string::operator=(const ft::string& string) {
	free(_buffer);
	_buffer = ft_strdup(string._buffer);
	_size = string._size;
	return (*this);
}
