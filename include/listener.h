/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listener.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/01 11:34:08 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENER_H
# define LISTENER_H

# include "server.h"
# include "tool.h"

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
