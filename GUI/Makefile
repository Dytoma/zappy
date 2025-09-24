##
## EPITECH PROJECT, 2025
## B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
## File description:
## Makefile GUI
##

SRC				=		$(shell find ./ -name "*.cpp"	\
							-not -path "./temporary/*")


OBJ				:=		$(SRC:.cpp=.o)


NAME			:=		zappy_gui


JUNK_FILES		=		$(shell find ./ -name "*~"	\
						-o -name "*.log"	\
						-o -name "vgcore*"	\
						-o -name "*.gcda"	\
						-o -name "*.gcno")


CPPFLAGS				:=		-std=c++20 -Wall -Wextra -Werror	\
								-Iinclude	\

RAYFLAGS		=	-lraylib -lGL -lm -lpthread -ldl -lrt -lX11

CXX					:= 		g++


all: $(NAME)


$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(CPPFLAGS) $(RAYFLAGS)


%.o: %.c
	$(CXX) $(CPPFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ)
	rm -f $(JUNK_FILES)


fclean: clean
	rm -f $(NAME)


re: fclean $(NAME)


.PHONY: all clean fclean re
