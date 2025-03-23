/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 02:55:34 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_H
# define EVENT_H

# include "server.h"

class Event
{
  public:
	virtual ~Event(void);
	virtual void Callback(short revents) = 0;
	class Server;
	class Client;
};

class Event::Server : public Event
{
  public:
	Server(int socket);
	virtual ~Server(void);
	virtual void Callback(short revents);

  private:
	int socket_;
};

class Event::Client : public Event
{
  public:
	Client(void);
	virtual ~Client(void);
	virtual void Callback(short revents);
};

#endif /* EVENT_H */
