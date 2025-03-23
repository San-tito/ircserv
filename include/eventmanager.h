/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventmanager.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 01:53:40 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTMANAGER_H
# define EVENTMANAGER_H

# include <cstdlib>
# include <poll.h>
# include <vector>

class EventManager
{
  public:
	EventManager(void);
	~EventManager(void);

	int Dispatch(void);

  private:
	std::vector<struct pollfd> events_;
};

#endif /* EVENTMANAGER_H */
