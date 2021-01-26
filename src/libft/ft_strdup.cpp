/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 20:30:31 by imicah            #+#    #+#             */
/*   Updated: 2021/01/26 20:31:36 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.hpp"

char*	ft_strdup(const char* src) {
	const size_t	len = ft_strlen(src);
	char*			result;

	result = new char[len + 1];
	ft_memcopy(src, result, len);
	result[len] = 0;
	return (result);
}