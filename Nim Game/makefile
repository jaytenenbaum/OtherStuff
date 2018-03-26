CFLAGS= -Wall -pedantic-errors -std=c99  -g  -lm 

all: nim

clean:
	-rm nim.o nim

nim: nim.o
	gcc $(CFLAGS) -o  nim nim.o

nim.o: nim.c
	gcc $(CFLAGS) -c  nim.c
