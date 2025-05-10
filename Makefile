CC = gcc
CFLAGS = -Iinclude -I/opt/homebrew/Cellar/csfml/2.6.1_1/include -Wall -Wextra
LDFLAGS = -L/opt/homebrew/Cellar/csfml/2.6.1_1/lib -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -lcsfml-network

SRC = src/main.c src/event_handler.c src/map.c src/transformations.c src/ui.c src/camera.c src/save_load.c src/update.c src/audio.c
OBJ = $(SRC:.c=.o)
EXEC = bin/main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(EXEC)

re: fclean all
