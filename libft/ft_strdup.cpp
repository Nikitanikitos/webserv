/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 10:41:22 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 14:53:32 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.hpp"

char* ft_memdup(const char* src, int size) {
	char	*result;
	size_t	i;

	i = 0;
	if (!(result = (char*)malloc(sizeof(char) * size)))
		return (0);
	for (; i < size; ++i)
		result[i] = src[i];
	return (result);
}
