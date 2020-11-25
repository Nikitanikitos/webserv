/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 14:17:15 by nikita            #+#    #+#             */
/*   Updated: 2020/11/25 04:21:46 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseConfigFile.hpp"

ParseConfigFile::ParseConfigFile(std::string& filename) :_filename(filename) { }

VirtualServer	ParseConfigFile::_parse_vs_directive() {
	return (VirtualServer());
}

Location			ParseConfigFile::_parse_location_directive() {
	return (Location());
}

std::vector<VirtualServer> ParseConfigFile::parse_file() {
	return (std::vector<VirtualServer>());
}
