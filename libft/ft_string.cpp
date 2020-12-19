/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 11:41:09 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 12:15:23 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.hpp"

ft::string ft::string::append(const std::string& string) {
	append(string.c_str(), string.size());
	return (*this);
}

const char*		ft::string::c_str() {
	return (_buffer);
}

ft::string		ft::string::append(const char* string, int i) {
	if (!_buffer) {
		_buffer = ft_strdup(string);
		_size = ft_strlen(_buffer);
	}
	else {
		char*	temp_buff;
		int 	q;

		q = 0;
		temp_buff = _buffer;
		_buffer = new char[_size + i + 1];
		while (*temp_buff)
			_buffer[q++] = *(temp_buff++);
		while (*string)
			_buffer[_size++] = *(string++);
		_buffer[_size] = '\0';
	}
	return (*this);
}

ft::string		ft::string::append(string string) {
	append(string.c_str(), string.size());
	return (*this);
}

size_t ft::string::size() {
	return (_size);
}

void ft::string::clear() {
	free(_buffer);
	_size = 0;
	_buffer = 0;
}
