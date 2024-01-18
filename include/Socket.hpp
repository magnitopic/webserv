/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:58:32 by jsarabia          #+#    #+#             */
/*   Updated: 2024/01/18 14:56:57 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

class Socket
{
private:
public:
	Socket(void);
	Socket(const Socket &socket);
	Socket &operator=(const Socket &socket);
	~Socket(void);
};

#endif
