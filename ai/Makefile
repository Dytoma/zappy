SRC = main.py

EXEC = zappy_ai

default: all

all:
	cp $(SRC) $(EXEC)
	chmod +x $(EXEC)

zappy_ai:
	cp $(SRC) $(EXEC)
	chmod +x $(EXEC)

run:
	./$(EXEC) $(ARGS)

clean:
	rm -f $(EXEC)

fclean: clean

re: fclean all