CC = gcc
CFLAGS = -Wall -Wextra -g `pkg-config --cflags raylib`
LDFLAGS = `pkg-config --libs raylib` -lm

SRC = main.c inimigo.c projetil.c
OBJ = $(SRC:.c=.o)
TARGET = cowboy_game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c inimigo.h player.h projetil.h
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f $(OBJ) $(TARGET)