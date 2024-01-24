/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:45:54 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/24 14:47:50 by alaparic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP_
#define LOCATION_HPP_

#include <iostream>

class Location
{
private:
	std::string path; // path
	bool autoIndex; // autoindex
	std::string index; // index
	std::string root; // root
	std::vector<std::string> methods; // methods
	
	
public:
	Location();
	Location(const Location &copy);
	Location &operator=(const Location &assign);
	~Location();

	
};

#endif
