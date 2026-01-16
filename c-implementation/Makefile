# Makefile for Checkers with Alpha-Beta Pruning

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = -lm

TARGET = checkers
SOURCES = main.c board.c ai.c input.c output.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = board.h ai.h input.h output.h

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
