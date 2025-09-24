##
## EPITECH PROJECT, 2025
## B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
## File description:
## Makefile server
##

SRC				=		$(shell find ./ -name "*.c"	\
							-not -path "./temporary/*")


OBJ				:=		$(SRC:.c=.o)


NAME			:=		zappy_server


HEADERS_PATH	=		-Iinclude/


JUNK_FILES		=		$(shell find ./ -name "*~"	\
						-o -name "*.log"	\
						-o -name "vgcore*"	\
						-o -name "*.gcda"	\
						-o -name "*.gcno")


CFLAGS				:=		-Wall -Wextra -ggdb3 -lm	\


CC					:= 		gcc


all: $(NAME)


$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) $(HEADERS_PATH) -c $< -o $@


clean:
	rm -f $(OBJ)
	rm -f $(JUNK_FILES)


fclean: clean
	rm -f $(NAME)


re: fclean $(NAME)


.PHONY: all clean fclean re
