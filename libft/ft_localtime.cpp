/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_localtime.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 02:04:31 by imicah            #+#    #+#             */
/*   Updated: 2020/11/25 02:09:07 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>

#define SEC_PER_DAY		86400
int 	get_number_days(time_t &seconds) {
	int 	result;

	result = (int)((float)seconds / SEC_PER_DAY);
	seconds -= result * SEC_PER_DAY;
	return (result);
}

#define SEC_PER_HOUR	3600
int		get_number_hours(time_t& seconds) {
	int 	result;

	result = (int)((float)seconds / SEC_PER_HOUR);
	seconds -= result * SEC_PER_HOUR;
	return (result);
}

#define SEC_PER_LEAP_YEAR	31622400
#define SEC_PER_YEAR		31536000
int 	get_number_years(time_t &seconds, int& is_leap_year) {
	int 	result;
	int 	current_year;

	result = 0;
	while (seconds > SEC_PER_YEAR) {
		if (is_leap_year == 3) {
			current_year = SEC_PER_LEAP_YEAR;
			is_leap_year = 0;
		}
		else {
			current_year= SEC_PER_YEAR;
			is_leap_year++;
		}
		result++;
		seconds -= current_year;
	}
	return (result);
}

#define SEC_PER_31DAY		2678400
#define SEC_PER_30DAY		2592000
#define SEC_PER_29DAY		2505600
#define SEC_PER_28DAY		2419200
int 	get_number_months(time_t &seconds, int is_leap_year) {
	int		number_month;
	int 	result;

	number_month = result = 0;
	while (seconds > SEC_PER_31DAY) {
		if (number_month == 1)
			seconds -= (is_leap_year) ? SEC_PER_28DAY : SEC_PER_29DAY;
		else if (number_month == 7)
			seconds -= SEC_PER_31DAY;
		else if (number_month % 2 == 0)
			seconds -= SEC_PER_31DAY;
		else
			seconds -= SEC_PER_30DAY;
		result++;
		number_month++;
	}
	return (result);
}

void	ft_localtime(tm& datetime, time_t seconds) {
	int		is_leap_year = 1;

	datetime.tm_year = get_number_years(seconds, is_leap_year);
	datetime.tm_mon = get_number_months(seconds, is_leap_year);
	datetime.tm_mday = get_number_days(seconds);
	datetime.tm_hour = get_number_hours(seconds);
}
