/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 12:50:34 by imicah            #+#    #+#             */
/*   Updated: 2020/12/19 12:15:23 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_LIBFT_HPP
# define WEBSERV_LIBFT_HPP

# include <cstddef>
# include <string>
# include <sys/time.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <thread>
# include <fcntl.h>
# include <zconf.h>

# define HTTP_VERSION	(std::string)"HTTP/1.1"
# define SERVER_VERSION	"WebServ/0.1"
# define CRLF			"\r\n"
# define SP				" "

char*					ft_strdup(const char *src);

namespace ft {
	struct		string {
	private:
		size_t			_size;
		char*			_buffer;

	public:
		string() : _size(0), _buffer() { }
		string(const ft::string& string) : _size(string._size), _buffer(string._buffer) { }
		string(const std::string& string) : _size(string.size())
			{ _buffer = ft_strdup(string.c_str()); }

		string		append(const std::string& string);
		ft::string	append(const char* string, int i);
		string		append(string string);
//		string		erase(size_t pos = 0, size_t n = -1);

		size_t			size();
		void			clear();
		const char*		c_str();
	};
}

void*					ft_memset(void*, int, size_t);

uint16_t				ft_htons(uint16_t);
uint32_t				ft_htonl(uint32_t);

int						ft_atoi(const char* nptr);

void					ft_localtime(tm&, time_t);

std::string				ft_getdate(struct timeval& tv);
std::string				ft_getfile(const char *path);

void*					worker(void* arg);
void*					ft_memcpy(void *dest, const void *src, size_t byte_sizes);

bool					ft_getline(int fd, std::string &line);

size_t					ft_strlen(const char *str);

char*					ft_realloc(char* str, size_t n);
char*					ft_strjoin(char const *s1, char const *s2);

#endif //WEBSERV_LIBFT_HPP
