/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 12:50:34 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 14:47:35 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_LIBFT_HPP
# define WEBSERV_LIBFT_HPP

# include <string>
#include <sys/stat.h>
# include "bytes.hpp"
#include "stdint.h"

struct					t_stat {
	struct stat			info;
	int					exists;
};

char*					ft_itoa(int n);

uint16_t				ft_htons(uint16_t);
uint32_t				ft_htonl(uint32_t);

int						ft_atoi(const char* nptr);

void					ft_localtime(tm&, time_t);

std::string				ft_getdate(struct timeval& tv);

bytes					ft_getfile(const char *path);

void*					worker(void* arg);
void*					ft_memset(void*, int, size_t);

bool					ft_getline(int fd, std::string &line);

#endif //WEBSERV_LIBFT_HPP
