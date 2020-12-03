/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 12:50:34 by imicah            #+#    #+#             */
/*   Updated: 2020/12/03 01:33:48 by imicah           ###   ########.fr       */
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

# define HTTP_VERSION	(std::string)"HTTP/1.1"
# define SERVER_VERSION	"WebServ/0.1"
# define CRLF			"\r\n"
# define SP				" "

void*		ft_memset(void*, int, size_t);

uint16_t	ft_htons(uint16_t);
uint32_t	ft_htonl(uint32_t);

int			ft_atoi(const char* nptr);

void		ft_localtime(tm&, time_t);

std::string		ft_getdate();

#endif //WEBSERV_LIBFT_HPP
