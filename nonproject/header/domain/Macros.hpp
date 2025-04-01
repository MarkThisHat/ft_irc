#pragma once

#ifndef MACROS_HPP
# define MACROS_HPP


/* General Responses */

# define RPL_WELCOME(recipient,client)                              "001 " + recipient + " :Welcome to the ft_irc network " + client
# define RPL_YOURHOST(recipient,server)                             "002 " + recipient + " :Your host is " + server + ", running version 1.0"
# define RPL_CREATED(recipient,datetime)                            "003 " + recipient + " :This server was created " + datetime
# define RPL_MYINFO(recipient,server)                               "004 " + recipient + " " + server + " 1.0 itkol"

# define RPL_LISTSTART(recipient)                                   "321 " + recipient + " Channel :Users Name"
# define RPL_LIST(recipient, channel, clientcount, topic)           "322 " + recipient + " " + channel + " " + clientcount + " :" + topic
# define RPL_LISTEND(recipient)                                     "323 " + recipient + " :End of /LIST"

# define RPL_CHANNELMODEIS(recipient, channel, modes, args)         "324 " + recipient + " " + channel + " " + modes + " " + args

# define RPL_NOTOPIC(recipient, channel)                            "331 " + recipient + " " + channel + " :No topic is set"
# define RPL_TOPIC(recipient, channel, topic)                       "332 " + recipient + " " + channel + " " + topic
# define RPL_TOPICWHOTIME(recipient, channel, nick, datetime)       "333 " + recipient + " " + channel + " " + nick + " " + datetime

# define RPL_INVITING(recipient, nick, channel)                     "341 " + recipient + " " + nick + " " + channel
# define RPL_INVITELIST(recipient, channel)                         "346 " + recipient + " " + channel
# define RPL_ENDOFINVITELIST(recipient)                             "347 " + recipient + " :End of /INVITE list"

# define RPL_WHOREPLY(recipient, channel, users)                    "352 " + recipient + " " + channel + " " + users // "<channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>"
# define RPL_ENDOFWHO(recipient, channel)                           "315 " + recipient + " " + channel + " :End of WHO list"

# define RPL_NAMREPLY(recipient, channel, users)                    "353 " + recipient + " = " + channel + " :" + users // "<symbol> <channel> :[prefix]<nick>"
# define RPL_ENDOFNAMES(recipient, channel)                         "366 " + recipient + " " + channel + " :End of /NAMES list"

# define RPL_INFO(recipient, info)                                  "371 " + recipient + " :" + info
# define RPL_ENDOFINFO(recipient)                                   "374 " + recipient + " :End of /INFO list"

# define RPL_HELPSTART(subject, text)                               "704 " + subject + " :" + text  //"<client> <subject> :<first line of help section>"
# define RPL_HELPTXT(subject, text)                                 "705 " + subject + " :" + text  //"<client> <subject> :<line of help text>"
# define RPL_ENDOFHELP(subject, text)                               "706 " + subject + " :" + text  //"<client> <subject> :<last line of help text>"


/* Error Responses */

# define ERR_NOSUCHNICK(recipient, nickname)                "401 " + recipient + " " + nickname + " :No such nick"
# define ERR_NOSUCHCHANNEL(recipient, channel)              "403 " + recipient + " " + channel + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(recipient, channel)           "404 " + recipient + " " + channel + " :Cannot send to channel"
# define ERR_TOOMANYCHANNELS(recipient, channel)            "405 " + recipient + " " + channel + " :You have joined too many channels"
# define ERR_UNKNOWNCOMMAND(recipient, command)             "421 " + recipient + " " + command + " :Unknown command"
# define ERR_NONICKNAMEGIVEN(recipient)                     "431 " + recipient + " :Nickname not given"
# define ERR_NICKNAMEINUSE(recipient, nickname)             "433 " + recipient + " " + nickname  + " :Nickname is already in use"
# define ERR_USERNOTINCHANNEL(recipient, nickname, channel) "441 " + recipient + " " + nickname + " " + channel + " :They aren't on that channel"
# define ERR_NOTONCHANNEL(recipient, channel)               "442 " + recipient + " " + channel + " :You're not on that channel"
# define ERR_USERONCHANNEL(recipient, user, channel)        "443 " + recipient + " " + user + " " + channel + " :is already on channel"
# define ERR_NOTREGISTERED(recipient)                       "451 " + recipient + " :You have not registered"
# define ERR_NEEDMOREPARAMS(recipient, command)             "461 " + recipient + " " + command + " :Not enough parameters"
# define ERR_ALREADYREGISTERED(recipient)                   "462 " + recipient + " :Unauthorized command (already registered)"
# define ERR_PASSWDMISMATCH(recipient)                      "464 " + recipient + " :Password is incorrect"
# define ERR_CHANNELISFULL(recipient, channel)              "471 " + recipient + " " + channel + " :Cannot join channel (+l)"
# define ERR_UNKNOWNMODE(recipient, channel)                "472 " + recipient + " " + channel + " :is unknown mode char to me"
# define ERR_INVITEONLYCHAN(recipient, channel)             "473 " + recipient + " " + channel + " :Cannot join channel (+i)"
# define ERR_BANNEDFROMCHAN(recipient, channel)             "474 " + recipient + " " + channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(recipient, channel)              "475 " + recipient + " " + channel + " :Cannot join channel (+k)"
# define ERR_CHANOPRIVSNEEDED(recipient, channel)           "482 " + recipient + " " + channel + " :You're not channel operator"


/* Command Responses */
/*
# define RPL_JOIN(client, channel, target)                  ":" + client + " JOIN " + channel + " * :" + target
# define RPL_PART(client, channel)                          ":" + client + " PART " + channel
# define RPL_PRIVMSG(client, target, message)               ":" + client + " PRIVMSG " + target + " :" + message
# define RPL_QUIT(client, message)                          ":" + client + " QUIT " + message
# define RPL_KICK(client, channel, target, reason)          ":" + client + " KICK " + channel + " " + target + " :" + reason
# define RPL_MODE(client, channel, modes, args)             ":" + client + " MODE " + channel + " " + modes + " " + args
# define RPL_NICK(client)                                   ":" + client + " NICK is now " + client
# define RPL_NOTICE(client, target, message)                ":" + client + " PRIVMSG " + target + " :" + message
# define RPL_PING(client, token)                            ":" + client + " PONG :" + token
*/
# define RPL_JOIN(client, channel)                        ":" + client + " JOIN " + channel
# define RPL_PART(client, channel)                        ":" + client + " PART " + channel /*+ " :" + reason*/
# define RPL_PRIVMSG(client, target, message)             ":" + client + " PRIVMSG " + target + " :" + message
# define RPL_QUIT(client, message)                        ":" + client + " QUIT" + message
# define RPL_KICK(client, channel, target, reason)        ":" + client + " KICK " + channel + " " + target + " :" + reason
# define RPL_MODE(client, channel, modes, args)           ":" + client + " MODE " + channel + " " + modes + " " + args
# define RPL_NICK(old_nick, new_nick)                     ":" + old_nick + " NICK " + new_nick
# define RPL_NOTICE(client, target, message)              ":" + client + " NOTICE " + target + " :" + message
# define RPL_PING(token)                                  "PONG :ft_irc :" + token


/* Broadcast messages */
/*
# define MESSAGE_CLIENT_JOIN(channel, client)                    client + " joined the channel " + channel
# define MESSAGE_CLIENT_LEAVE(channel, client)                   client + " left " + client + " from channel " + channel
# define MESSAGE_CLIENT_KICK(channel, client, target, reason)    client + " kicked " + target + " from channel " + channel + " for reason: " + reason
# define MESSAGE_ADMIN_CHANGE(channel, client)                   client + " is now the admin of the channel " + channel
*/
# define MESSAGE_CLIENT_JOIN(channel, client)                 client + " joined " + channel
# define MESSAGE_CLIENT_LEAVE(channel, client)                client + " left " + channel
# define MESSAGE_CLIENT_KICK(channel, client, target, reason) client + " kicked " + target + " from " + channel + " " + reason
# define MESSAGE_ADMIN_CHANGE(channel, client)                client + " is now an operator in " + channel



#endif // MACROS_HPP




/* 
On RPL_NAMREPLY <symbol> notes the status of the channel. It can be one of the following:

    ("=", 0x3D) - Public channel.

Prefixos para usuários:

    ~ para dono do canal    (+q)
    352 irc.example.com #nome_do_canal user1 203.0.113.1 user1 * @ irc.example.com 1 :John Doe

    @ para operadores do canal    (+o)
    352 irc.example.com #nome_do_canal user2 198.51.100.1 user2 * ~ irc.example.com 1 :Jane Smith

    usuários comuns não possuem symbol
    352 irc.example.com #nome_do_canal user3 203.0.113.2 user3 *   irc.example.com 1 :Carlos Silva
*/