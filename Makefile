NAME =	hotrace
CC	=	cc
CFLAGS	= -Wall -Wextra -Werror

#Colors
DEF_COLOR := \033[0;39m
GREEN := \033[0;92m

SRC	=	main.c getnextline.c hashing.c murmur.c utils.c hashmap_free.c \
		hashmap_get.c hashmap_insert.c

OBJ =	$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) hotrace.h
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "$(GREEN)Executable $(NAME) created!$(DEF_COLOR)"

clean:	
		rm -f *.o

fclean:	clean 
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
