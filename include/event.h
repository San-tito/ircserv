/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:33:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 11:12:58 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_H
# define EVENT_H

# include <poll.h>
# include <string>

class Event
{
  public:
	virtual ~Event(void);
	virtual void Callback(short revents) = 0;
};

class NewConnection : public Event
{
  public:
	NewConnection(int socket);
	virtual void Callback(short revents);

  private:
	int socket_;
};

class ClientSession : public Event
{
  public:
	ClientSession(int socket);
	virtual void Callback(short revents);

  private:
	int socket_;
};

#endif /* EVENT_H */
