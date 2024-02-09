/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostReq.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:19:39 by jsarabia          #+#    #+#             */
/*   Updated: 2024/02/09 19:56:24 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POSTREQ_HPP_
#define POSTREQ_HPP_

#include "webserv.hpp"

/**
 * This class will store the information needed to upload files when performing a POST
*/
class PostReq
{
private:
	// atributes
	std::string	fileName;
	std::string	content;

public:
	// orthodox canonical form

	PostReq();
	PostReq(const PostReq &copy);
	PostReq &operator=(const PostReq &assign);
	~PostReq();
	// setters and getters

	void		setFileName(std::string request);
	std::string	getFileName();
	void		setFileContent(std::string request);

};

#endif
