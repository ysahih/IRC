#define RPL_WELCOME(nick, hostname)                                         ":" + hostname + " 001 " + nick + " :Welcome " + nick + " to the ft_irc network !\r\n"
#define RPL_YOURHOST(nick, hostname)                                        ":" + hostname + " 002 " + nick + " :Your host is " + hostname + " running version 1.0 !\r\n"
#define RPL_CREATED(nick, hostname)                                         ":" + hostname + " 003 " + nick + " :This server was created 2023-9-15 !\r\n"
#define RPL_MYINFO(nick, hostname)                                          ":" + hostname + " 004 " + nick + " :Host: " + hostname + ", Version: 1.0, User mode: none, Channel modes: o, t, k, i !\r\n"


#define ERR_PASSWDMISMATCH(nick, hostname)	                                ":" + hostname + " 464 " + nick + " :Password incorrect !\r\n"
#define ERR_ALREADYREGISTERED(nick, hostname)                      	        ":" + hostname + " 462 " + nick + " :You may not reregister !\r\n"


#define ERR_ERRONEUSNICKNAME(nick, hostname)	                            ":" + hostname + " 432 " + nick + " :Erroneus nickname !\r\n"
#define ERR_NONICKNAMEGIVEN(nick, hostname)	                                ":" + hostname + " 431 " + nick + " :No nickname given !\r\n"
#define ERR_NICKNAMEINUSE(nick, hostname)                                   ":" + hostname + " 433 " + nick + " :Nickname is already in use !\r\n"
#define RPL_NICKCHANGE(oldNick, nick, hostname)                             ":" + oldNick + " NICK "  + nick + "\r\n"


#define ERR_ALREADYREGISTERED(nick, hostname)                      	        ":" + hostname + " 462 " + nick + " :You may not reregister !\r\n"
#define ERR_NEEDMOREPARAMS(nick, hostname)                                  ":" + hostname + " 461 " + nick + " :Not enough parameters !\r\n"


    
# define ERR_BADCHANNELMASK(nick, hostname, mask, correction)               ":" + hostname + " 476 " + nick + " " + mask + " :Invalid channel mask." + correction + "\r\n"
# define ERR_CHANNELISFULL(nick, hostname)                                  ":" + hostname + " 471 " + channelName + " :Cannot join channel (+l)\r\n"
# define ERR_BADCHANNELKEY(nick, hostname, channelName)                     ":" + hostname + " 475 " + nick + " " + channelName + " :Cannot join channel (+K) - bad key\r\n"
# define ERR_INVITEONLY(nick, hostname)                                     ":" + hostname + " 473 " + channelName + " :Cannot join channel (+i)\r\n"
    
# define RPL_UMODEIS(hostname, channelname)                                 ":" + hostname + " MODE " + channelname + " +nt\r\n"
# define RPL_JOIN(nick, username, channelname, ipaddress)                   ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
# define RPL_TOPIC(hostname, topic, nick)                                   ":" + hostname + " 332 " + nick + " " + channelName + " " + topic + "\r\n"
# define RPL_TOPICWHOTIME(topicsetter, time, nick, hostname, channelName)   ":" + hostname + " 333 " + nick + " " + channelName + " " + topicsetter + "!~" + topicsetter + "@" + hostname + " " + time + "\r\n"
# define RPL_NAMREPLY(hostname, clients, channelname,nick)                  ":" + hostname + " 353 " + nick + " = " + channelname + " :" + clients + "\r\n"
# define RPL_ENDOFNAMES(hostname, nick, channelname)                        ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"


# define RPL_MODEIS(channel, hostname, mode)                                ":" + hostname + " MODE " + channel + " " + mode + "\r\n"
# define ERR_INVALIDMODEPARAM(channel, hostname, flag)                      ":" + hostname +  " 696 " + channel + " " + flag + " * you must specifiy a parameter for the op mode\r\n"
# define ERR_USERNOTINCHANNEL(hostname, channel)                            ":" + hostname + " 441 " + channel + " "  + ":they aren't on that channel\r\n"
# define ERR_NOTOP(hostname, channel)                                       ":" + hostname + " 482 " + channel + " "  + ":You're not a channel operator\r\n"


# define RPL_CREATIONTIME(hostname, channelName, nick, time)               ":" + hostname + " 329 " + nick + " " + channelName + " " + time + "\r\n"
# define RPL_CHANNELMODES(hostname, channelName, nick, channelmode)        ":" + hostname + " 324 " + nick + " " + channelName + " " + channelmode + "\r\n"


# define RPL_INVITING(hostname, inviting, invited, channel)                 ":" + hostname + " 341 " + inviting + " " + invited + " " + channel + " :Inviting " + invited + " to " + channel + "\r\n"
# define RPL_INVITE(nick, username ,clienthostname, invited, channel)       ":" + nick + "!" + username + "@" + clienthostname + " INVITE " + invited + " :" + channel + "\r\n"
# define ERR_NOTONCHANNEL(hostname, channel)                                ":" + hostname + " 442 " + channel + " "  + ":You're not on that channel\r\n"
# define ERR_NOSUCHCHANNEL(hostname, channel, nick)                         ":" + hostname + " 403 " + nick + " " + channel + " :No such channel\r\n"
# define ERR_NOSUCHNICK(hostname, channel, argument)                        ":" + hostname + " 401 " + channel + " " +  argument + " :No such nick/channel\r\n"
# define ERR_USERONCHANNEL(hostname, channel, nick)                         ":" + hostname + " 443 " + nick + " " + channel + "  :is already on channel\r\n"


#define ERR_INPUTTOOLONG(nick, hostname) ":" + hostname + " 417 " + nick + " :Input line was too long !\r\n"
 

# define RPL_VIEWTOPIC(hostname, nick, channel, topic)                      ":" + hostname + " 332 " + nick + " " + channel + " :" + topic + "\r\n" 
# define RPL_VIEWTIME(hostname, nick, channel, time)                        ":" + hostname + " 333 " + nick + " " + channel + " " + nick + " " + time + "\r\n"
# define RPL_SETTOPIC(nick, hostname, channel, topic)                       ":" + nick + "!" + nick + "@" + hostname + " TOPIC " + channel + " :" + topic + "\r\n"

# define RPL_BOT(hostname, nick, message)                      ":" + hostname + " 001 " + nick + " Dad joke: " +  message + "\r\n" 
# define ERR_BOT(hostname, nick)                      ":" + hostname + " 450 " + nick + "enter [BOT generate] to generate a joke" + "\r\n" 






#define ERR_NEEDMOREPARAMS(nick, hostname) ":" + hostname + " 461 " + nick + " :Not enough parameters !\r\n"
#define ERR_NOSUCHCHANNELl(nick, hostname) ":" + hostname + " 403 " + nick + " :No such channel !\r\n"
#define ERR_CHANOPRIVSNEEDED(nick, hostname) ":" + hostname + " 482 " + nick + " :You're not channel operator !\r\n"
#define ERR_USERNOTINCHANNELl(nick, hostname) ":" + hostname + " 441 " + nick + " :They aren't on that channel !\r\n"
#define ERR_NOTONCHANNELl(nick, hostname) ":" + hostname + " 442 " + nick + " :You're not on that channel !\r\n"

#define ERR_KICKREPLY(nick, name, kicked, channelName, reason) ":"+nick+"!~"+nick+"@localhost" + " KICK " + channelName + " " + kicked + " :"+reason+"\r\n"


#define ERR_NOSUCHNICKl(nick, hostname) ":" + hostname + " 401 " + nick + " :No such nick/channel\r\n"
#define PRVMSG_RPL(nick, reciever, hostname, message) ":" + nick + " PRIVMSG " + reciever + " :" + message + "\r\n"
#define RPL_AWAY(nick, hostname) ":" + hostname + " 401 " + nick + " :No such nick/channel\r\n"
#define PRVMSG_CHRPL(nick, reciever, hostname, message) ":" + nick + "!~"+nick+"@localhost PRIVMSG " + reciever + " :"+ message+"\r\n"


#define QUIT_RPL(nick, reason) ":" + nick + "!~"+nick+"@localhost QUIT :"+ reason+"\r\n"



#define PART_RPL(nick, channelName) ":"+nick+"!~"+nick+"@localhost" + " PART " + channelName + "\r\n"