/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:33:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 11:52:57 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_H
# define USER_H

# include <ctime>
# include <string>

# define READBUFFER_LEN 2048
# define MAX_COMMANDS 3
# define COMMAND_LEN 512

class User
{
  public:
	User(int socket);
	~User(void);

	void Read(void);
	void Write(void);
	void Write(std::string const &msg);
	void Write(std::string const &prefix, std::string const &msg);
	void WritePrefix(std::string const &msg);
	void WriteErrNeedMoreParams(std::string const &command);
	void WriteErrNotRegistered(void);
	void Request(void);
	int socket(void) const;
	bool registered(void) const;
	time_t last_activity(void) const;
	std::string rbuf(void) const;
	std::string wbuf(void) const;
	std::string nickname(void) const;
	std::string mask(void) const;

  private:
	int socket_;
	bool registered_;
	time_t last_activity_;
	std::string rbuf_;
	std::string wbuf_;
	std::string nickname_;
	std::string username_;
	std::string password_;
	std::string hostname_;
};

#endif /* USER_H */
