/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostReq.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:19:39 by jsarabia          #+#    #+#             */
/*   Updated: 2024/03/07 15:57:30 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	std::string	postType;
	std::string	boundary;

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
	std::string	getFileContent();
	void		setPostType(std::string type);
	std::string	getPostType();
	void		setBoundary(std::string request);
	std::string	getBoundary();
	void		setContentTextPlain(std::string str);

};

#endif
