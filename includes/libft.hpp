/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 12:50:34 by imicah            #+#    #+#             */
/*   Updated: 2020/11/22 15:49:23 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_LIBFT_HPP
# define WEBSERV_LIBFT_HPP

# include <cstddef>

void*		ft_memset(void*, int, size_t);

uint16_t	ft_htons(uint16_t);
uint32_t	ft_htonl(uint32_t);

int			ft_atoi(const char* nptr);

#endif //WEBSERV_LIBFT_HPP
