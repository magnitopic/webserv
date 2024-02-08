/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 19:37:25 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/08 15:27:03 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

void	Response::setResponseNotFound()
{
	this->response = "<html><head><title>Error 404</title></head><body><h1>Ups, this page was absorbed by the Black Hole</h1>";
	this->response += "<img src=\"../../../images/marvin.png\" width='530' height='400'></img></body></html>";
}
