/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 19:37:25 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/07 21:04:35 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

void	Response::setResponseNotFound()
{
	this->response = "<html><head><title>Error 404</title></head><body><h1>Ups, this page was absorbed by the Black Hole</h1>";
	this->response += "<img src=\"https://cdn.dribbble.com/users/3818458/screenshots/13795625/media/7c35f1f62b95c9f624d2cb0759efd450.png?resize=400x0\" width='530' height='400'></img></body></html>";
}
