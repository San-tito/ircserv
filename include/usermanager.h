/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usermanager.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 16:15:03 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERMANAGER_H
# define USERMANAGER_H

# include "typedefs.h"
# include "user.h"
# include <map>

# define TIMEOUT 20

class UserManager
{
  public:
	UserManager(void);
	~UserManager(void);

	void AddUser(int fd);
	void DelUser(int fd);
	User *Search(const std::string &name);
	void Read(void);
	void Read(int fd);
	void Write(void);
	void Write(int fd);
	void CheckTimeouts(void);

  private:
	std::map<int, User *> users;
};

#endif /* USERMANAGER_H */
