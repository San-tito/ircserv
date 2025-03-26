#ifndef CHANNEL_H
# define CHANNEL_H
# include "channelmanager.h"
# include "typedefs.h"
# include "user.h"
# include <cstddef>
# include <map>
# include <set>
# include <string>
# include <vector>

class Channel
{
  public:
	Channel(const std::string &name);
	~Channel();
	std::string name() const;
	std::set<char> modes() const;
	std::string topic() const;
	std::string key() const;
	std::map<std::string, User *> users() const;
	size_t maxUsers() const;
	bool isInvited(User *user) const;
	bool isOperator(User *user) const;
	void maxUsers(size_t max_users);
	void topic(const std::string &topic);
	void key(const std::string &key);
	void AddMode(char mode);
	bool HasMode(char mode) const;
	void DelMode(char mode);
	void AddInvite(User *user);
	void AddUser(User *user);
	void RemoveUser(User *user);
	void AddOperator(User *user);
	void RemoveOperator(User *user);
	void Write(User *sender, const std::string &message);
	User *SearchUser(const std::string &name);

	void Join(User *user);
	void Part(User *user, const std::string &reason);
	void Kick(User *user, User *target, const std::string &reason);
	void Mode(User *user, std::vector<std::string> &params);

  private:
	std::string name_;
	std::set<char> modes_;
	std::string topic_;
	std::string key_;
	size_t max_users_;
	std::map<std::string, User *> users_;
	std::map<std::string, User *> invites_;
	std::map<std::string, User *> operators_;
};
#endif /* CHANNEL_H */
