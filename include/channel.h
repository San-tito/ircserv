#ifndef CHANNEL_H
# define CHANNEL_H
# include "channelManager.h"
# include "typedefs.h"
# include "user.h"
# include <cstddef>
# include <set>
# include <string>
# include <vector>

class Channel
{
  public:
	Channel(const std::string &name);
	~Channel();
	std::string getName() const;
	std::set<char> getModes() const;
	std::string getTopic() const;
	std::string getKey() const;
	size_t getMaxUsers() const;
	std::vector<Client *> getClients() const;
	void setMaxUsers(size_t max_users);
	void setTopic(const std::string &topic);
	void setKey(const std::string &key);
	void addMode(char mode);
	bool hasMode(char mode) const;
	void delMode(char mode);
	void addInvite(Client *client);
	bool isInvited(Client *client) const;
	void addClient(Client *client);
	void removeClient(Client *client);
	bool isOperator(Client *client) const;
	void addOperator(Client *client);
	void removeOperator(Client *client);
	void Write(Client *sender, const std::string &message);
	Client *SearchUser(const std::string &name);

	void Join(Client *client);
	void Part(Client *client, const std::string &reason);
	void Kick(Client *client, Client *target, const std::string &reason);
	void Mode(Client *client, std::vector<std::string> &params);

  private:
	std::string name_;
	std::set<char> modes_;
	std::string topic_;
	std::string key_;
	size_t max_users_;
	std::vector<Client *> clients_;
	std::vector<Client *> invites_;
	std::set<Client *> operators_;
};
#endif /* CHANNEL_H */
