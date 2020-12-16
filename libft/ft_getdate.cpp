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
	long 			cur_time;
	std::string 	res;

	ft_memset(&buffer, 0, 100);
	ft_memset(&timeinfo, 0, sizeof(timeinfo));
	ft_memset(&ft_timeinfo, 0, sizeof(ft_timeinfo));
	gettimeofday(&tv, nullptr);
	cur_time = tv.tv_sec + (tv.tv_usec / 1000000);

	timeinfo = localtime(&cur_time);
	ft_localtime(ft_timeinfo, cur_time);
//	std::cout << "TIME INFO year: " << timeinfo->tm_year << std::endl;
//	std::cout << "TIME INFO month: " << timeinfo->tm_mon << std::endl;
//	std::cout << "TIME INFO day: " << timeinfo->tm_mday << std::endl;
//	std::cout << "TIME INFO hour: " << timeinfo->tm_hour << std::endl;
//	std::cout << "TIME INFO min: " << timeinfo->tm_min << std::endl;
//	std::cout << "TIME INFO sec: " << timeinfo->tm_sec << std::endl;
//	std::cout << std::endl;
//	std::cout << "year: " << ft_timeinfo.tm_year << std::endl;
//	std::cout << "month: " << ft_timeinfo.tm_mon << std::endl;
//	std::cout << "day: " << ft_timeinfo.tm_mday << std::endl;
//	std::cout << "hour: " << ft_timeinfo.tm_hour << std::endl;
//	std::cout << "min: " << ft_timeinfo.tm_min << std::endl;
//	std::cout << "sec: " << ft_timeinfo.tm_sec << std::endl;
//	std::cout << std::endl;

	strftime (buffer, 100, "%a, %d %Y %H:%M:%S GMT", &ft_timeinfo);
//	std::cout << buffer << std::endl;
//	strftime (buffer, 100, "%a, %d %Y %H:%M:%S GMT", timeinfo);
//	std::cout << buffer << std::endl;
	return (std::string(buffer));
}