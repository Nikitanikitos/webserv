/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getdate.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 17:01:43 by imicah            #+#    #+#             */
/*   Updated: 2020/12/19 13:53:36 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.hpp"

std::string		ft_getdate(struct timeval& tv) {
	struct tm		ft_timeinfo;
	char			buffer[100];
	long 			cur_time;

	ft_memset(&ft_timeinfo, 0, sizeof(ft_timeinfo));
	cur_time = tv.tv_sec + (tv.tv_usec / 1000000);

	ft_localtime(ft_timeinfo, cur_time);

	strftime (buffer, 100, "%a, %d %Y %H:%M:%S GMT", &ft_timeinfo);
	return (std::string(buffer));
}
