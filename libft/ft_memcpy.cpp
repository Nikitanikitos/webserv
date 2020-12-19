/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 10:27:52 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 10:29:23 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.hpp"

void	*ft_memcpy(void *dest, const void *src, size_t byte_sizes)
{
	const char	*temp_src = (char*)src;
	char		*temp_dest;

	temp_dest = (char*)dest;
	if (temp_dest == temp_src || byte_sizes == 0)
		return (dest);
	while (byte_sizes--)
		*(temp_dest++) = *(temp_src++);
	return (dest);
}