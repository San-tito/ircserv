/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usermanager.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 17:56:16 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERMANAGER_H
# define USERMANAGER_H

# include "typedefs.h"
# include "user.h"
# include <map>

# define TIMEOUT 20
# define MAX_NICK_LEN 9

class UserManager
{
  public:
	UserManager(void);
	~UserManager(void);

	void AddUser(int fd);
	void DelUser(int fd);
	User *Search(const std::string &name);
	bool IsValidNick(std::string nick);
	void Read(void);
	void Read(int fd);
	void Write(void);
	void Write(int fd);
	void CheckTimeouts(void);

  private:
	std::map<int, User *> users;
};

#endif /* USERMANAGER_H */
