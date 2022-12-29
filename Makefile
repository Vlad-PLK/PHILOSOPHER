# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/30 13:09:38 by vpolojie          #+#    #+#              #
#    Updated: 2022/12/23 15:14:58 by vpolojie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS = philo.c ft_utils.c

OBJS = ${SRCS:.c=.o}

CC = gcc

RM = rm -f

CFLAGS = -Wall -Wextra -Werror

.c.o:
	${CC} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
				${CC} -g $^ -o $@

all:	${NAME};

clean: 	
		${RM} ${OBJS}

fclean: clean
		${RM} ${NAME}

re:		fclean all

.PHONY:		all clean fclean re