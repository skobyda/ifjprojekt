#TODO
CC = gcc
CFLAGS = -pedantic -Wall -Wextra
OTHERCFILES = $(wildcard ../[^main]*.c)
CFILES = parsertest.c $(OTHERCFILES)
OFILES = $(CFILES:.c=.o)

all: parsertest part-clean

parsertest: $(OFILES)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm parsertest
