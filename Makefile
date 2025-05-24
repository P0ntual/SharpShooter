CC = gcc
CFLAGS = -Wall -Wextra -g `pkg-config --cflags raylib` -Iincludes
LDFLAGS = `pkg-config --libs raylib` -lm

SRC_DIR = src
OBJ_DIR = build

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/inimigo.c $(SRC_DIR)/projetil.c $(SRC_DIR)/ataque.c $(SRC_DIR)/player.c $(SRC_DIR)/round.c

OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

TARGET = cowboy_game

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c includes/player.h includes/inimigo.h includes/projetil.h includes/ataque.h includes/round.h
$(OBJ_DIR)/inimigo.o: $(SRC_DIR)/inimigo.c includes/inimigo.h includes/player.h
$(OBJ_DIR)/projetil.o: $(SRC_DIR)/projetil.c includes/projetil.h includes/player.h
$(OBJ_DIR)/ataque.o: $(SRC_DIR)/ataque.c includes/ataque.h includes/inimigo.h includes/projetil.h includes/player.h
$(OBJ_DIR)/player.o: $(SRC_DIR)/player.c includes/player.h
$(OBJ_DIR)/round.o: $(SRC_DIR)/round.c includes/round.h includes/inimigo.h

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)