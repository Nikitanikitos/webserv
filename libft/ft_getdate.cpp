/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getdate.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 17:01:43 by imicah            #+#    #+#             */
/*   Updated: 2020/12/02 17:02:14 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.hpp"

std::string		ft_getdate() {
	struct tm		timeinfo;
	struct timeval	tv;
	char			buffer[100];

	ft_memset(&buffer, 0, 100);
	gettimeofday(&tv, nullptr);
	ft_localtime(timeinfo, tv.tv_sec);
	strptime (buffer, "Now it's %I:%M%p.", &timeinfo);

	return (std::string(buffer));
}