CC = gcc
CFLAGS = -Wall -Wextra -g `pkg-config --cflags raylib`
LDFLAGS = `pkg-config --libs raylib` -lm

SRC = main.c inimigo.c projetil.c ataque.c player.c round.c
OBJ = $(SRC:.c=.o)
TARGET = cowboy_game

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

main.o: main.c player.h inimigo.h projetil.h ataque.h round.h
inimigo.o: inimigo.c inimigo.h player.h
projetil.o: projetil.c projetil.h player.h
ataque.o: ataque.c ataque.h inimigo.h projetil.h player.h
player.o: player.c player.h
round.o: round.c round.h inimigo.h

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)