/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getfile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 02:11:18 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 15:17:35 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <fcntl.h>
# include <zconf.h>
# include "bytes.hpp"

bytes	ft_getfile(const char *path) {
	const int 	fd = open(path, O_RDONLY);
	bytes		result;
	int			bytes;
	char		buff[2049];

	while ((bytes = read(fd, &buff, 2048)) > 0)
		result.add(buff, bytes);
	close(fd);
	return (result);
}
