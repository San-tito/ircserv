/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listener.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 16:26:58 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENER_H
# define LISTENER_H

# include "connection.h"
# include "server.h"

class Listener
{
  public:
	Listener(int port, std::string &listen_addr);
	~Listener(void);

  private:
	int sock_;
	struct sockaddr_in address_;
};

#endif /* LISTENER_H */
