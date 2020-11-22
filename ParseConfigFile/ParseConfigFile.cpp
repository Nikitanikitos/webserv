/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 14:17:15 by nikita            #+#    #+#             */
/*   Updated: 2020/11/22 14:20:45 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseConfigFile.hpp"

ParseConfigFile::ParseConfigFile(std::string& filename) :_filename(filename) { }

VirtualServer	ParseConfigFile::_parse_vs_directive() {
	return (VirtualServer());
}

Route			ParseConfigFile::_parse_route_directive() {
	return (Route());
}

std::vector<VirtualServer> ParseConfigFile::parse_file() {
	return (std::vector<VirtualServer>());
}
