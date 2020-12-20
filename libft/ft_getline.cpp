/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getline.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 00:50:11 by nikita            #+#    #+#             */
/*   Updated: 2020/12/07 01:13:05 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <zconf.h>
# include <string>

bool			ft_getline(int fd, std::string &line) {
	static std::string	surplus = std::string();
	char				buffer[513];
	size_t 				bytes;

	if (surplus.empty()) {
		while ((bytes = read(fd, buffer, 512)) > 0) {
			buffer[bytes] = '\0';
			surplus.append(buffer);
		}
		if (!bytes && surplus.empty())
			return (false);
	}
	std::string::size_type pos = surplus.find('\n');
	if (pos == std::string::npos) {
		line = surplus;
		surplus.clear();
	} else {
		line = surplus.substr(0, pos);
		surplus = surplus.substr(pos + 1);
	}
	return (true);
}
