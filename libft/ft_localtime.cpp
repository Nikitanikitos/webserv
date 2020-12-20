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

#include <sys/time.h>

#define SEC_PER_DAY		86400
int 	get_number_days(time_t &seconds) {
	int 	result = 0;

	while (seconds > SEC_PER_DAY) {
		seconds -= SEC_PER_DAY;
		++result;
	}
	return (result);
}

#define SEC_PER_HOUR	3600
int		get_number_hours(time_t& seconds) {
	int 	result = 0;

	while (seconds > SEC_PER_HOUR) {
		seconds -= SEC_PER_HOUR;
		++result;
	}
	return (result);
}

#define SEC_PER_MIN			60
int		get_number_min(time_t& seconds) {
	int result = 0;

	while (seconds > SEC_PER_MIN) {
		++result;
		seconds -= SEC_PER_MIN;
	}
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
	while (seconds > SEC_PER_28DAY) {
		if (number_month == 1)
			seconds -= (is_leap_year == 3) ? SEC_PER_29DAY : SEC_PER_28DAY;
		else if (number_month == 7)
			seconds -= SEC_PER_31DAY;
		else if (number_month > 7) {
			if (number_month % 2 == 0)
				seconds -= SEC_PER_30DAY;
			else
				seconds -= SEC_PER_31DAY;
		}
		else if (number_month % 2 == 0)
			seconds -= SEC_PER_31DAY;
		else
			seconds -= SEC_PER_30DAY;
		result++;
		number_month++;
	}
	return (result);
}

int 	get_name_of_day(tm& datetime) {
	int month_code;
	int year_code;
	int result;
	int core_year = datetime.tm_year + 1900;

	if (datetime.tm_mon == 9 || datetime.tm_mon == 0)
		month_code = 1;
	else if (datetime.tm_mon == 4)
		month_code = 2;
	else if (datetime.tm_mon == 7)
		month_code = 3;
	else if (datetime.tm_mon == 5)
		month_code = 5;
	else if (datetime.tm_mon == 11 || datetime.tm_mon == 8)
		month_code = 6;
	else if (datetime.tm_mon == 3 || datetime.tm_mon == 6)
		month_code = 0;
	else
		month_code = 4;
	year_code = (6 + (core_year % 100) + (core_year % 100) / 4) % 7;
	result = (datetime.tm_mday + month_code + year_code) % 7;
	if (core_year % 4 == 0)
		++result;
	return (result - 2 == -1 ? 6 : result - 2);
}

void	ft_localtime(tm& datetime, time_t seconds) {
	int		is_leap_year = 1;

	seconds += 2208988800;
	datetime.tm_year = get_number_years(seconds, is_leap_year);
	datetime.tm_mon = get_number_months(seconds, is_leap_year);
	datetime.tm_mday = get_number_days(seconds) + 1;
	datetime.tm_hour = get_number_hours(seconds) + 3;
	datetime.tm_min = get_number_min(seconds);
	datetime.tm_sec = seconds;
	datetime.tm_wday = get_name_of_day(datetime);
}
