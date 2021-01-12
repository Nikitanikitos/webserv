/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:30:05 by nikita            #+#    #+#             */
/*   Updated: 2020/12/21 12:37:06 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : autoindex(non_accepted), location_type(default_location)
	{ allow_methods.assign(count_methods, accepted); }

bool						Location::isAllowMethod(const std::string& method) const {
	static std::string	methods[count_methods] = {"GET", "HEAD", "POST", "PUT"};

	for (int i = 0; i < count_methods; ++i) {
		if (method == methods[i])
			return (allow_methods[i]);
	}
	return (false);
}
