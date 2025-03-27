/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:41:46 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 14:59:12 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_H
# define MESSAGES_H

# define RPL_WELCOME(nick, server) "001 " + (nick) + " :Welcome to the Internet Relay Network " + (server)
# define RPL_YOURHOST(nick, server, version) "002 " + (nick) + " :Your host is " + (server) + ", running version ircserv-" + (version)
# define RPL_CREATED(server, date) "003 " + (server) + " :This server has been started " + (date)
# define RPL_MYINFO(nick, server, version) "004 " + (nick) + " " + (server) + " ircserv-" + (version)
# define RPL_UMODEIS(client, modes) "221 " + (client) + " +" + (modes)
# define RPL_CHANNELMODEIS(channel, client, modes) "324 " + (channel) + " " + (client) + " +" + (modes)
# define RPL_NOTOPIC(channel, server) "331 " + (channel) + " " + (server) + " :No topic is set"
# define RPL_TOPIC(channel, server, topic) "332 " + (channel) + " " + (server) + " :" + (topic)
# define RPL_INVITING(client, channel, inviter) "341 " + (client) + " " + (channel) + " " + (inviter)
# define ERR_NOSUCHNICK(nick, target) "401 " + (nick) + " " + (target) + " :No such nick or channel name"
# define ERR_NOSUCHCHANNEL(channel, target) "403 " + (channel) + " " + (target) + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(channel, target) "404 " + (channel) + " " + (target) + " :Cannot send to channel"
# define ERR_NORECIPIENT(command) "411 " + (command) + " :No recipient given (" + (command) + ")"
# define ERR_NOTEXTTOSEND(command) "412 " + (command) + " :No text to send"
# define ERR_UNKNOWNCOMMAND(command, target) "421 " + (command) + " " + (target) + " :Unknown command"
# define ERR_ERRONEUSNICKNAME(nick, target) "432 " + (nick) + " " + (target) + " :Erroneous nickname"
# define ERR_NICKNAMETOOLONG(client, nick) "432 " + (client) + " " + (nick) + " :Nickname too long"
# define ERR_NICKNAMEINUSE(nick, target) "433 " + (nick) + " " + (target) + " :Nickname already in use"
# define ERR_NOTONCHANNEL(channel, nick) "442 " + (channel) + " " + (nick) + " :You are not on that channel"
# define ERR_CLIENTNOTINCHANNEL(channel, nick, target) "441 " + (channel) + " " + (nick) + " " + (target) + " :They aren't on that channel"
# define ERR_NOTREGISTERED(nick) "451 " + ((nick).empty() ? "*" : nick) + " :Connection not registered"
# define ERR_NEEDMOREPARAMS(nick, param) "461 " + ((nick).empty() ? "*" : nick) + " " + (param) + " :Syntax error"
# define ERR_ALREADYREGISTRED(command) "462 " + (command) + " :Connection already registered"
# define ERR_CHANNELISFULL(channel, target) "471 " + (channel) + " " + (target) + " :Cannot join channel (+l) -- Channel is full, try later"
# define ERR_UNKNOWNMODE(channel, mode, target) "472 " + (channel) + " " + (mode) + " :is unknown mode char for " + (target)
# define ERR_INVITEONLYCHAN(channel, target) "473 " + (channel) + " " + (target) + " :Cannot join channel (+i) -- Invited clients only"
# define ERR_BADCHANNELKEY(channel, key) "475 " + (channel) + " " + (key) + " :Cannot join channel (+k) -- Wrong channel key"
# define ERR_NOCHANMODES(channel) "477 " + (channel) + " :Channel doesn't support modes"
# define ERR_CHANOPRIVSNEEDED(channel, target) "482 " + (channel) + " " + (target) + " :You are not channel operator"
# define ERR_CHANOPPRIVTOOLOW(channel, target) "482 " + (channel) + " " + (target) + " :Your privileges are too low"
# define ERR_CLIENTSDONTMATCH(client) "502 " + (client) + " :Can't set/get mode for other clients"
# define ERR_INVALIDMODEPARAM(channel, mode, param) "696 " + (channel) + " " + (mode) + " " + (param) + " :Invalid mode parameter"

#endif /* MESSAGES_H */
