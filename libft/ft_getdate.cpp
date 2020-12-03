/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getdate.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 17:01:43 by imicah            #+#    #+#             */
/*   Updated: 2020/12/02 17:07:34 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.hpp"

std::string		ft_getdate() {
	struct tm		ft_timeinfo;
	struct tm*		timeinfo;
	struct timeval	tv;
	char			buffer[100];

	ft_memset(&buffer, 0, 100);
	ft_memset(&timeinfo, 0, sizeof(timeinfo));
	ft_memset(&ft_timeinfo, 0, sizeof(ft_timeinfo));
	gettimeofday(&tv, nullptr);

	timeinfo = localtime(&tv.tv_sec);
	ft_localtime(ft_timeinfo, tv.tv_sec);

	strptime (buffer, "Now it's %I:%M%p.", &ft_timeinfo);

	return (std::string(buffer));
}