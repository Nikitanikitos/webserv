/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 11:41:09 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 17:26:03 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bytes.hpp"

void			bytes::add(const std::string& string) { add(string.c_str(), string.size()); }
void			bytes::add(const bytes& string) { add(string.c_str(), string.size()); }

void			bytes::add(const char* string, size_t i) {
	char*	temp_buff;

	if (!size_) {
		delete [] buffer;
		buffer = bytedup(string, i);
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
	buffer = new char[1];
}

void bytes::erase(size_t n) {

	if (n >= size_)
		clear();
	else {
		bytes	temp_buff = *this;
		size_ -= n;
		temp_buff.buffer += n;
		buffer = bytedup(temp_buff, size_);
		temp_buff.buffer -= n;
	}
}

bytes&		bytes::operator=(const bytes& string) {
	delete []buffer;
	buffer = bytedup(string, string.size_);
	size_ = string.size_;
	return (*this);
}

char*		bytes::bytedup(const bytes& src, size_t size) {
	char	*result;
	int 	i;

	size = (size < src.size_) ? size : src.size_;
	result = new char[size + 1];
	for (i = 0; i < size; ++i)
		result[i] = src.buffer[i];
	result[i] = 0;
	return (result);
}

size_t		bytes::find(const char* needle) {
	for (int i = 0; i < size_; ++i) {
		if (buffer[i] == *needle) {
			for (int j = 0; needle[j]; ++j)
				if (buffer[i + j] != needle[j])
					break;
			return (i);
		}
	}
	return (-1);
}

bytes bytes::substr(size_t i) {
	bytes	result;

	result.buffer = bytedup(*this, i);
	result.size_ = (i < size_) ? i : size_;
	return (result);
}

char* bytes::bytedup(const char* src, size_t size) {
	char	*result;
	int 	i;

	result = new char[size + 1];
	for (i = 0; i < size; ++i)
		result[i] = src[i];
	result[i] = 0;
	return (result);
}

void bytes::rtrim(size_t n) {
	char*	temp_buff = buffer;

	size_ -= n;
	buffer = new char[size_ + 1];
	buffer = bytedup(temp_buff, size_);
	delete []temp_buff;
}
