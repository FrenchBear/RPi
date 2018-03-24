CC = gcc
CFLAGS = -Wall -std=gnu11
LDLIBS = -lm

make:	utf8code

utf8code:	utf8code.o utf8.o
			$(CC) utf8code.o utf8.o $(LDLIBS) -o $@

clean:
		rm utf8code
