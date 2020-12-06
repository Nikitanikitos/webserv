/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getfile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 02:11:18 by nikita            #+#    #+#             */
/*   Updated: 2020/12/06 02:22:00 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.hpp"

std::string		ft_getfile(const char *path) {
	const int 			fd = open(path, O_RDONLY);
	std::string			result;
	char				buff[512];

	while ((read(fd, &buff, 512)) > 0) {
		result.append(buff);
		ft_memset(&buff, 0, 512);
	}
	close(fd);
	return (result);
}
