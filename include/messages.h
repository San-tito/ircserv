/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:41:46 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 12:15:48 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_H
# define MESSAGES_H

# define RPL_WELCOME(nick, server) "001 " + (nick) + " :Welcome to the Internet Relay Network " + (server)
# define RPL_YOURHOST(nick, server, version) "002 " + (nick) + " :Your host is " + (server) + ", running version ircserv-" + (version)
# define RPL_CREATED(server, date) "003 " + (server) + " :This server has been started " + (date)
# define RPL_MYINFO(nick, server, version) "004 " + (nick) + " " + (server) + " ircserv-" + (version)
# define RPL_UMODEIS(user, modes) "221 " + (user) + " +" + (modes)
# define RPL_CHANNELMODEIS(channel, user, modes) "324 " + (channel) + " " + (user) + " +" + (modes)
# define RPL_NOTOPIC(channel, server) "331 " + (channel) + " " + (server) + " :No topic is set"
# define RPL_TOPIC(channel, server, topic) "332 " + (channel) + " " + (server) + " :" + (topic)
# define RPL_INVITING(user, channel, inviter) "341 " + (user) + " " + (channel) + " " + (inviter)
# define ERR_NOSUCHNICK(nick, target) "401 " + (nick) + " " + (target) + " :No such nick or channel name"
# define ERR_NOSUCHCHANNEL(channel, target) "403 " + (channel) + " " + (target) + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(channel, target) "404 " + (channel) + " " + (target) + " :Cannot send to channel"
# define ERR_NORECIPIENT(command) "411 " + (command) + " :No recipient given (" + (command) + ")"
# define ERR_NOTEXTTOSEND(command) "412 " + (command) + " :No text to send"
# define ERR_UNKNOWNCOMMAND(command, target) "421 " + (command) + " " + (target) + " :Unknown command"
# define ERR_ERRONEUSNICKNAME(nick, target) "432 " + (nick) + " " + (target) + " :Erroneous nickname"
# define ERR_NICKNAMETOOLONG(nick, max_len) "432 " + (nick) + " " + (nick) + " :Nickname too long, max. " + std::to_string(max_len) + " characters"
# define ERR_NICKNAMEINUSE(nick, target) "433 " + (nick) + " " + (target) + " :Nickname already in use"
# define ERR_NOTONCHANNEL(channel, nick) "442 " + (channel) + " " + (nick) + " :You are not on that channel"
# define ERR_USERNOTINCHANNEL(channel, nick, target) "441 " + (channel) + " " + (nick) + " " + (target) + " :They aren't on that channel"
# define ERR_NOTREGISTERED(command) "451 " + (command) + " :Connection not registered"
# define ERR_NEEDMOREPARAMS(command, param) "461 " + (command) + " " + (param) + " :Syntax error"
# define ERR_ALREADYREGISTRED(command) "462 " + (command) + " :Connection already registered"
# define ERR_CHANNELISFULL(channel, target) "471 " + (channel) + " " + (target) + " :Cannot join channel (+l) -- Channel is full, try later"
# define ERR_UNKNOWNMODE(channel, mode, target) "472 " + (channel) + " " + (mode) + " :is unknown mode char for " + (target)
# define ERR_INVITEONLYCHAN(channel, target) "473 " + (channel) + " " + (target) + " :Cannot join channel (+i) -- Invited users only"
# define ERR_BADCHANNELKEY(channel, key) "475 " + (channel) + " " + (key) + " :Cannot join channel (+k) -- Wrong channel key"
# define ERR_NOCHANMODES(channel) "477 " + (channel) + " :Channel doesn't support modes"
# define ERR_CHANOPRIVSNEEDED(channel, target) "482 " + (channel) + " " + (target) + " :You are not channel operator"
# define ERR_CHANOPPRIVTOOLOW(channel, target) "482 " + (channel) + " " + (target) + " :Your privileges are too low"
# define ERR_USERSDONTMATCH(user) "502 " + (user) + " :Can't set/get mode for other users"
# define ERR_INVALIDMODEPARAM(channel, mode, param) "696 " + (channel) + " " + (mode) + " " + (param) + " :Invalid mode parameter"

#endif /* MESSAGES_H */
