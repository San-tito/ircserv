#ifndef CHANNELMANAGER_H
# define CHANNELMANAGER_H

# include "typedefs.h"
# include <map>
# include <string>
# include <vector>

class ChannelManager
{
  public:
	ChannelManager(void);
	~ChannelManager(void);

	Channel *Search(const std::string &name);
	bool AddChannel(Channel *channel);
	bool IsValidName(const std::string &name);
	void RemoveChannel(const std::string &name);
	void Mode(Client *client, const std::vector<std::string> &params);
	bool Join(Client *client, const std::string &name);
	void Part(Client *client, const std::string &channelName,
		const std::string &reason);
	void PartAll(Client *client);
	void Kick(Client *client, const std::string &nick,
		const std::string &channelName, const std::string &reason);

  private:
	std::map<std::string, Channel *> channels_;
};

#endif /* CHANNELMANAGER_H */
