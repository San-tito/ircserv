/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:00 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/22 20:47:21 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "typedefs.h"
# include <algorithm>
# include <cfloat>
# include <climits>
# include <csignal>
# include <cstdarg>
# include <cstring>
# include <iostream>
# include <map>
# include <set>
# include <sstream>
# include <string>
# include <vector>

class Server
{
  public:
	Server(int port, std::string password);
	~Server(void);

	void Exit(int status);

	static Server *instance;
};

#endif /* IRCD_HPP */
