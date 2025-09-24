##
## EPITECH PROJECT, 2024
## MAKEFILE
## File description:
## MAKEFILE
##

NAME_SERVER			:=		zappy_server


NAME_GUI			:=		zappy_gui


NAME_AI				:=		zappy_ai


SERVER				:=		./server


GUI					:=		./GUI

AI					:=		./ai


all: $(NAME_SERVER) $(NAME_GUI) $(NAME_AI)


$(NAME_SERVER):
	make -C $(SERVER)
	mv $(SERVER)/$(NAME_SERVER) ./


$(NAME_GUI):
	make -C $(GUI)
	mv $(GUI)/$(NAME_GUI) ./


$(NAME_AI):
	make -C $(AI)
	mv $(AI)/$(NAME_AI) ./


clean:
	make clean -C $(SERVER)
	make clean -C $(GUI)
	make clean -C $(AI)


fclean:
	make fclean -C $(SERVER)
	make fclean -C $(GUI)
	make fclean -C $(AI)
	rm -f $(NAME_SERVER) $(NAME_GUI) $(NAME_AI)


re: fclean all


.PHONY: all clean fclean re
