/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:41:46 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 11:52:36 by sguzman          ###   ########.fr       */
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

#endif /* MESSAGES_H */
