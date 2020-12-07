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

# include "libft.hpp"

static void		read_all_file(int fd, std::string& static_buff) {
	char	buffer[512];

	while (read(fd, buffer, 512) > 0) {
		static_buff.append(buffer);
		ft_memset(buffer, 0, 512);
	}
	static_buff.append("\0");
}

bool			ft_getline(int fd, std::string &line) {
	static std::string		static_buff;
	size_t					pos;

	if (static_buff.empty())
		read_all_file(fd, static_buff);

	pos = static_buff.find('\n');
	if (pos == std::string::npos) {
		line = static_buff;
		static_buff.clear();
		return (false);
	}
	else {
		line = static_buff.substr(0, pos);
		static_buff.erase(0, pos);
		return (true);
	}
}
