# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: labderra <labderra@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/07 13:13:00 by labderra          #+#    #+#              #
#    Updated: 2024/08/07 13:37:43 by labderra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
SRC = philo.c \

CC = clang -Wall -Wextra -Werror -pthread -g -fsanitize=address
OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) -o $@

%.o : %.c
	$(CC) -c $< -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re 
