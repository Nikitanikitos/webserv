/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 10:35:39 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 10:35:39 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.hpp"

char	*ft_strjoin(char const *s1, char const *s2)
{
	const size_t	len_new_string = ft_strlen(s1) + ft_strlen(s2);
	size_t			i;
	char			*result_s;

	i = 0;
	result_s = (char*)malloc(sizeof(char) * (len_new_string + 1));
	if (!result_s)
		return (NULL);
	while (s1 && *s1)
		result_s[i++] = *(s1++);
	while (s2 && *s2)
		result_s[i++] = *(s2++);
	result_s[i] = '\0';
	return (result_s);
}