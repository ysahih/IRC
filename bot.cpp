/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idabligi <idabligi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:26:11 by idabligi          #+#    #+#             */
/*   Updated: 2023/11/01 15:15:32 by idabligi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::getDate(int fd)
{
    char buff[100];
    std::string time;
    std::time_t now_time_s = std::time(NULL);
    std::tm *localtime = std::localtime(&now_time_s);
    
    std::strftime(buff, sizeof(buff), "%d/%m/%Y---%H:%M:%S", localtime);
    time = buff;
    this->sendMessage(fd, ":BOT PRIVMSG " + this->list[fd].getNick() + " :" + "[" + time + "]" + "\r\n");

}

void    Server::getHelp(int fd)
{
    std::string man[20] = {"||----------------------------------------------------------------------------------||",
                           "||--------------------------------------MANUAL--------------------------------------||",
                           "||----------------------------------------------------------------------------------||",
                           "||                                                                                  ||",
                           "|| ∗ Set UserName   : USER + (your username)                                        ||",
                           "|| ∗ Set NickName   : NICK + (your nickname)                                        ||",
                           "|| ∗ Join a channel : JOIN + #(channel name)                                        ||",
                           "|| ∗ KICK - Eject a client from the channel                                         ||",
                           "|| ∗ INVITE - Invite a client to a channel                                          ||",
                           "|| ∗ TOPIC - Change or view the channel topic                                       ||",
                           "|| ∗ MODE - Change the channel’s mode:                                              ||",
                           "||   · i: Set/remove Invite-only channel                                            ||",
                           "||   · t: Set/remove the restrictions of the TOPIC command to channel operators     ||",
                           "||   · k: Set/remove the channel key (password)                                     ||",
                           "||   · o: Give/take channel operator privilege                                      ||",
                           "||   · l: Set/remove the user limit to channel                                      ||",
                           "||__________________________________________________________________________________||",
                           };
    for (int i = 0; i < 17; i++)
        this->sendMessage(fd, ":BOT PRIVMSG " + this->list[fd].getNick() + " :" + man[i] + "\r\n");
}


void Server::bot(int fd, std::stringstream &iss)
{
    int i;
    std::string str;
    std::string str2;
    std::string check[] = {"DATE", "HELP"};
    iss >> str;
    iss >> str2;
    if (!iss.eof())
        return (this->sendMessage (fd, "421 " + str2 + " :Unknown command\r\n"));
    for (i = 0; i < 3; i++)
    {
        if (str == check[i])
        break ;
    }
    switch (i)
    {
        case 0:
            getDate(fd);
            break;
        case 1:
            getHelp(fd);
            break;
        default:
            this->sendMessage (fd, "421 " + str + " :Unknown command\r\n");
    }
}