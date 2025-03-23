/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventmanager.cc                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 01:44:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 01:51:01 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eventmanager.h"

EventManager::EventManager(void)
{
}

EventManager::~EventManager(void)
{
}

int EventManager::Dispatch(void)
{
	int i(poll(events_.data(), events_.size(), 1000));
	int processed(0);
	for (size_t index = 0; index < events_.size() && processed < i; index++)
	{
		struct pollfd &event(events_[index]);
		if (event.revents)
			processed++;
		if (event.revents & POLLIN)
		{
		}
		if (event.revents & POLLOUT)
		{
		}
	}
	return (i);
}
