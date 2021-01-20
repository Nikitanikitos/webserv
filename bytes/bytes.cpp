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

void			bytes::add(const char* string, size_t i) {
	const char*		temp_buff = buffer;

	if (size_ + i >= capacity) {
		capacity = (size_ + i) * 2;
		buffer = new char[capacity];
		ft_memcopy(temp_buff, buffer, size_);
		delete []temp_buff;
	}
	ft_memcopy(string, buffer + size_, i);
	size_ += i;
	buffer[size_] = 0;
}

void			bytes::clear() {
	delete []buffer;
	size_ = 0;
	capacity = 10;
	buffer = new char[capacity];
}

void			bytes::erase(size_t n) {
	char*	temp_buff = buffer;

	if (n >= size_)
		clear();
	else {
		capacity = capacity - n + 10;
		size_ -= n;
		buffer = new char[capacity];
		ft_memcopy(temp_buff + n, buffer, size_);
		buffer[size_] = 0;
		delete[] temp_buff;
	}
}

bytes&		bytes::operator=(const bytes& string) {
	delete []buffer;
	capacity = string.capacity;
	size_ = string.size();
	buffer = new char[capacity];
	ft_memcopy(string.c_str(), buffer, size_);
	buffer[size_] = 0;
	return (*this);
}

size_t		bytes::find(const char* needle) const {
	for (int i = 0; i < size_; ++i) {
		if (buffer[i] == *needle) {
			int 	j;
			for (j = 0; needle[j]; ++j)
				if (buffer[i + j] != needle[j]) break;
			if (!needle[j])	return (i);
		}
	}
	return (-1);
}

size_t		bytes::rfind(const char* needle) const {
	int i = size_;

	i--;
	for (; i >= 0; --i) {
		if (buffer[i] == *needle) {
			int 	j;
			for (j = 0; needle[j]; ++j)
				if (buffer[i + j] != needle[j]) break;
			if (!needle[j])	return (i);
		}
	}
	return (-1);
}

void	bytes::rtrim(size_t n) {
	size_ -= n;
	buffer[size_] = 0;
}
