/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventmanager.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 12:33:47 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTMANAGER_H
# define EVENTMANAGER_H

# include "event.h"
# include "typedefs.h"
# include <cstdlib>
# include <map>
# include <poll.h>
# include <vector>

class EventManager
{
  public:
	EventManager(void);
	~EventManager(void);

	template <typename EventType> void AddEvent(int fd, short events)
	{
		struct pollfd p;
		p.fd = fd;
		p.events = events;
		p.revents = 0;
		pollfds_.push_back(p);
		events_[fd] = new EventType(fd);
	}

	void DelEvent(int fd);
	int Dispatch(void);

  private:
	std::map<int, Event *> events_;
	std::vector<struct pollfd> pollfds_;
};

#endif /* EVENTMANAGER_H */
