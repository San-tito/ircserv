/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventmanager.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 11:23:30 by sguzman          ###   ########.fr       */
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

	void AddClientSession(int fd, short events);
	void AddNewConnection(int fd, short events);
	void DelEvent(int fd);
	void MaskEvent(int fd, short events);
	void UnmaskEvent(int fd, short events);
	int Dispatch(void);

  private:
	std::map<int, Event *> events_;
	std::vector<struct pollfd> pollfds_;
};

#endif /* EVENTMANAGER_H */
