/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 11:41:09 by nikita            #+#    #+#             */
/*   Updated: 2020/12/21 13:02:11 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bytes.hpp"

void			bytes::add(const std::string& string) { add(string.c_str(), string.size()); }
void			bytes::add(const bytes& string) { add(string.c_str(), string.size()); }

const char*		bytes::c_str() const { return (buffer); }
size_t			bytes::size() const { return (size_); }

void			bytes::add(const char* string, int i) {
	char*	temp_buff;

	if (!buffer) {
		buffer = _bytedup(string, i);
		size_ = i;
	}
	else {
		temp_buff = buffer;
		buffer = new char[size_ + i + 1];
		for (int j = 0; j < size_ ; ++j)
			buffer[j] = temp_buff[j];
		for (int k = 0; k < i; ++k)
			buffer[size_++] = string[k];
		buffer[size_] = 0;
		delete []temp_buff;
	}
}

void			bytes::clear() {
	delete []buffer;
	size_ = 0;
	buffer = 0;
}

void			bytes::erase(size_t pos, size_t n)
	{ if (n >= size_) clear(); }

bytes&		bytes::operator=(const bytes& string) {
	delete []buffer;
	buffer = _bytedup(string.buffer, string.size_);
	size_ = string.size_;
	return (*this);
}

char*		bytes::_bytedup(const char* src, int size) {
	char	*result;
	int 	i;

	result = new char[size + 1];
	for (i = 0; i < size; ++i)
		result[i] = src[i];
	result[i] = 0;
	return (result);
}
