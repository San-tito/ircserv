/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usermanager.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 13:50:06 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERMANAGER_H
# define USERMANAGER_H

# include "typedefs.h"
# include "user.h"
# include <map>

class UserManager
{
  public:
	UserManager(void);
	~UserManager(void);

	void AddUser(int fd);
	void DelUser(int fd);
	void Read(int fd);
	void Write(int fd);

  private:
	std::map<int, User *> users;
};

#endif /* USERMANAGER_H */
