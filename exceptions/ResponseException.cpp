/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseException.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 16:20:35 by imicah            #+#    #+#             */
/*   Updated: 2020/12/17 13:13:22 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseException.hpp"

ResponseException::ResponseException(const std::string& status_code, const std::string& message_phrase,
									 const std::string& error_page) : Response(status_code, message_phrase), _error_page(error_page) { }
