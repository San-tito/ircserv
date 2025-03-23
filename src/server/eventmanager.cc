/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventmanager.cc                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 01:44:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 02:42:50 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eventmanager.h"

EventManager::EventManager(void)
{
}

EventManager::~EventManager(void)
{
	std::map<int, Event *>::iterator it(events_.begin());
	for (; it != events_.end(); it++)
		delete (it->second);
}

void EventManager::AddEvent(int fd, short events, Event *event)
{
	struct pollfd	p;

	p.fd = fd;
	p.events = events;
	p.revents = 0;
	pollfds_.push_back(p);
	events_[fd] = event;
}

void EventManager::DelEvent(int fd)
{
	for (size_t index = 0; index < pollfds_.size(); index++)
	{
		if (pollfds_[index].fd == fd)
		{
			pollfds_.erase(pollfds_.begin() + index);
			break ;
		}
	}
	delete (events_[fd]);
	events_.erase(fd);
}

int EventManager::Dispatch(void)
{
	int i(poll(pollfds_.data(), pollfds_.size(), 1000));
	int processed(0);
	for (size_t index = 0; index < pollfds_.size() && processed < i; index++)
	{
		struct pollfd &event(pollfds_[index]);
		if (event.revents)
		{
			events_[event.fd]->Callback(event.revents);
			processed++;
		}
	}
	return (i);
}
