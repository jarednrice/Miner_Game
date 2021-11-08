# compiler
CC = gcc
# debugging info and compiler warnings
CFLAGS = -g -Wall
# c files
CFILES = main.c player_map_info/player_and_map.c graphics/graphics.c
# libraries
LIBS = -lncurses

prog:
	$(CC) $(CFLAGS) $(CFILES) -o miner $(LIBS)
