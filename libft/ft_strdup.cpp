/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 10:41:22 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 10:41:22 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.hpp"

char	*ft_strdup(const char *src)
{
	char	*result;
	size_t	i;

	i = 0;
	if (!(result = (char*)malloc(sizeof(char) * (ft_strlen(src) + 1))))
		return (0);
	while (*src)
		result[i++] = *(src++);
	result[i] = '\0';
	return (result);
}
