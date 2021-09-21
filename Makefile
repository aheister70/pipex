# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: aheister <aheister@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2021/09/10 15:45:24 by aheister      #+#    #+#                  #
#    Updated: 2021/09/16 14:38:04 by aheister      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME			=	pipex

CFLAGS			=	-Wall -Werror -Wextra -Iincludes
CC				=	gcc

LIBFT			=	lib/libft/libft.a

SRCS			= 	srcs/errors.o\
					srcs/free.o\
					srcs/initialize.o\
					srcs/pipex.o

HEADER_FILES	=	includes/pipex.h

%.o:				%.c $(HEADER_FILES)
					$(CC) -c $(CFLAGS) -o $@ $<	

OBJ_FILES 		= 	$(SRCS)

all:				$(NAME)

$(LIBFT):		
					$(MAKE) -C lib/libft

$(NAME):			$(OBJ_FILES) $(LIBFT)
					$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(LIBFT)

clean:
					rm -f $(SRCS)
					$(MAKE) clean -C lib/libft

fclean:				clean
					rm -f $(NAME)
					rm -f $(LIBFT)

re:
					$(MAKE) fclean
					$(MAKE) all

.PHONY:				all clean fclean re
