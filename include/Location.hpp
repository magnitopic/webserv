/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 12:11:11 by jsarabia          #+#    #+#             */
/*   Updated: 2024/01/30 12:11:20 by jsarabia         ###   ########.fr       */
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