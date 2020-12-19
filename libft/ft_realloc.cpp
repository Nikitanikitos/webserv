/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 10:23:09 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 10:32:49 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.hpp"

char*	ft_realloc(char* str, size_t n) {
	char	*temp;

	temp = str;
	if (str == 0)
		str = (char*)malloc(sizeof(char) * n);
	else {
		int 	len = ft_strlen(str);
		str = (char*)malloc(sizeof(char) * len * n);
		ft_memcpy((void*)str, (void*)temp, len);
		free(temp);
	}
	return (str);
}
