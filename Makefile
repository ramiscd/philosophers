# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rdamasce <rdamasce@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/12 22:36:27 by rdamasce          #+#    #+#              #
#    Updated: 2026/05/12 22:36:31 by rdamasce         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = philo
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -pthread
SRC     = src/main.c src/utils.c src/init.c src/threads.c src/monitor.c
OBJ     = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
