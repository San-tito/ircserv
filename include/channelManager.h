#ifndef CHANNELMANAGER_H
# define CHANNELMANAGER_H

# include <string>
# include <vector>

typedef class User	Client;

class				Channel;

class ChannelManager
{
  public:
	ChannelManager();
	~ChannelManager();

	Channel *findChannel(const std::string &name);
	bool addChannel(Channel *channel);
    bool isValidName(const std::string &name);
	void removeChannel(const std::string &name);
	void ExecuteMode(Client *client, std::vector<std::string> &params);
	void ExecuteJoin(Client *client, const std::string &name);
	void ExecutePart(Client *client, const std::string &channelName,
		const std::string &reason);
	void ExecutePartAll(Client *client);
	void ExecuteKick(Client *client, const std::string &nick,
		const std::string &channelName, const std::string &reason);


  private:
	std::vector<Channel *> channels_;
};

#endif /* CHANNELMANAGER_H */
