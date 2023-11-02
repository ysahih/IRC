# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: idabligi <idabligi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/28 13:12:52 by idabligi          #+#    #+#              #
#    Updated: 2023/11/01 09:21:54 by idabligi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -f
HEADERS = Channel.hpp Client.hpp Server.hpp 

FL = main Channel Client Parser Server bot Commands/User Commands/Nick Commands/Invite Commands/Kick \
	Commands/Join Commands/Privmsg Commands/Topic Commands/Mode \

FO = $(FL:=.o)

all : $(NAME)

$(NAME) : $(FO) $(HEADERS)
	$(CC) $(FLAGS) $(FO) -o $(NAME)

%.o : %.cpp  $(HEADERS)
	$(CC) $(FLAGS) -o $@ -c $<

clean :
	$(RM) $(FO)

fclean : clean
	$(RM) $(NAME)

re : fclean all