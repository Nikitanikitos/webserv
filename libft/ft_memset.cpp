/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 12:47:51 by imicah            #+#    #+#             */
/*   Updated: 2020/11/19 12:52:29 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

void*	ft_memset(void* dest, int symb, size_t byte_sizes)
{
	for (char* temp_dest = (char *)dest; byte_sizes--; temp_dest++)
		*temp_dest = symb;
	return (dest);
}
