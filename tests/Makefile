#TODO
CC = gcc
CFLAGS = -pedantic -Wall -Wextra
OTHERCFILES = $(wildcard ../[^main]*.c)
CFILES = TestScanner.c $(OTHERCFILES)
OFILES = $(CFILES:.c=.o)

all: TestScanner part-clean

TestScanner: $(OFILES)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm TestScanner

part-clean:
	rm *.o
	rm ../*.o
