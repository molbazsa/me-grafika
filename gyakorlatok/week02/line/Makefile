CC = gcc
CFLAGS = -std=c11 -pedantic -Wall
LSDL = -lmingw32 -lSDL2main -lSDL2

SRC = src
OBJ = obj
BIN = bin
EXT = .exe

TARGETS = line

all: $(foreach target, $(TARGETS), $(BIN)/$(target)$(EXT))

$(BIN)/line$(EXT): $(SRC)/main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LSDL)

DIRS = $(OBJ) $(BIN)

dirs:
	mkdir $(DIRS)
