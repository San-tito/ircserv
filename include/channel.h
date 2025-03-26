#ifndef CHANNEL_H
# define CHANNEL_H
# include "channelmanager.h"
# include "typedefs.h"
# include "client.h"
# include <cstddef>
# include <map>
# include <set>
# include <string>
# include <vector>

class Channel
{
  public:
	Channel(const std::string &name);
	~Channel(void);
	std::string name() const;
	std::set<char> modes() const;
	std::string topic() const;
	std::string key() const;
	std::map<std::string, Client *> clients() const;
	size_t maxClients() const;
	bool isInvited(Client *client) const;
	bool isOperator(Client *client) const;
	void maxClients(size_t max_clients);
	void topic(const std::string &topic);
	void key(const std::string &key);
	void AddMode(char mode);
	bool HasMode(char mode) const;
	void DelMode(char mode);
	void AddInvite(Client *client);
	void AddClient(Client *client);
	void RemoveClient(Client *client);
	void AddOperator(Client *client);
	void RemoveOperator(Client *client);
	void Write(Client *sender, const std::string &message);
	Client *SearchClient(const std::string &name);

	void Join(Client *client);
	void Part(Client *client, const std::string &reason);
	void Kick(Client *client, Client *target, const std::string &reason);
	void Mode(Client *client, std::vector<std::string> &params);

  private:
	std::string name_;
	std::set<char> modes_;
	std::string topic_;
	std::string key_;
	size_t max_clients_;
	std::map<std::string, Client *> clients_;
	std::map<std::string, Client *> invites_;
	std::map<std::string, Client *> operators_;
};
#endif /* CHANNEL_H */
