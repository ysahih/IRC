# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: idabligi <idabligi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/28 13:12:52 by idabligi          #+#    #+#              #
#    Updated: 2023/10/28 14:42:35 by idabligi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
NAME2 = BOT

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -f
HEADERS = Channel.hpp Client.hpp Server.hpp

#Mandatory Files
FL = main Channel Client Parser Server\

FO = $(FL:=.o)

#Bonus Files
FLB = bot\

FOB = $(FLB:=.o)

all : $(NAME)

bonus : $(NAME2)

$(NAME) : $(FO) $(HEADERS)
	$(CC) $(FLAGS) $(FO) -o $(NAME)

$(NAME2) : $(FOB)
	$(CC) $(FLAGS) $(FOB) -o $(NAME2)

%.o:%.cpp $(HEADERS)
	$(CC) -c $(FLAGS) $<

clean :
	$(RM) $(FO) $(FOB)

fclean : clean
	$(RM) $(NAME) $(NAME2) $(FO) $(FOB)

re : fclean all