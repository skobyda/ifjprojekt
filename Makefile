#TODO
CC = gcc
CFLAGS = -pedantic -Wall -Wextra -g
CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)

all: ifj18 part-clean

ifj18: $(OFILES)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm ifj18

part-clean:
	rm *.o
