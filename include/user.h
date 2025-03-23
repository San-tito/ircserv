/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:33:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 13:51:32 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_H
# define USER_H

# include <ctime>
# include <string>

class User
{
  public:
	User(int socket);
	~User(void);

	void Read(void);
	void Write(void);

  private:
	int socket_;
	bool registered_;
	time_t last_activity_;
	std::string rbuf_;
	std::string wbuf_;
	std::string nickname_;
	std::string username_;
	std::string password_;
};

#endif /* USER_H */
