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

extern void			ft_memcopy(const void* src, void* dst, size_t size);

class	bytes {
private:
	size_t			size_;
	size_t			capacity;
	char*			buffer;

	inline void		byte_init(const char* data) {
		buffer = new char[capacity];
		ft_memcopy(data, buffer, size_);
		buffer[size_] = 0;
	}

public:
	bytes() : size_(0), capacity(10) { buffer = new char[capacity]; }
	~bytes() { delete []buffer; }

	bytes(const char* data, size_t size) : size_(size), capacity(size * 2) { byte_init(data); }
	bytes(const bytes& string) : size_(string.size_), capacity(string.capacity) { byte_init(string.c_str()); }
	bytes(const std::string& string) : size_(string.size()), capacity(string.size() * 2) { byte_init(string.c_str()); }

	bytes&			operator=(const bytes& string);

	inline void				add(const std::string& string) { add(string.c_str(), string.size()); }
	void					add(const char* string, size_t i);
	inline void				add(const bytes& string) { add(string.c_str(), string.size()); }
	void					erase(size_t n);
	void					rtrim(size_t n);

	void					clear();
	size_t					find(const char* needle) const ;
	size_t					rfind(const char* needle) const;

	inline bytes			substr(size_t i) { return (bytes(buffer, (i < size_) ? i : size_)); }
	inline size_t			size() const { return (size_); }
	inline bool				empty() const { return (!size_); }
	inline const char*		c_str() const { return (buffer); }
};

#endif //WEBSERV_BYTES_HPP
