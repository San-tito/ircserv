#ifndef CHANNELMANAGER_H
# define CHANNELMANAGER_H

# include "typedefs.h"
# include <map>
# include <string>
# include <vector>

class	Channel;

class ChannelManager
{
  public:
	ChannelManager(void);
	~ChannelManager(void);

	Channel *FindChannel(const std::string &name);
	bool AddChannel(Channel *channel);
	bool IsValidName(const std::string &name);
	void RemoveChannel(const std::string &name);
	void ExecuteMode(Client *client, std::vector<std::string> &params);
	void ExecuteJoin(Client *client, const std::string &name);
	void ExecutePart(Client *client, const std::string &channelName,
		const std::string &reason);
	void ExecutePartAll(Client *client);
	void ExecuteKick(Client *client, const std::string &nick,
		const std::string &channelName, const std::string &reason);

  private:
	std::map<std::string, Channel *> channels_;
};

#endif /* CHANNELMANAGER_H */
