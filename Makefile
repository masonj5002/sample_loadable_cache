
OBJS = main.o collatz.o cache.o
LIBS = libbozo_cache.so librex_cache.so libmemoize.so liblast_recently_used.so libpassthrough.so

all: main $(LIBS)

CC = gcc
CFLAGS = -Wall -ansi -std=c11 --pedantic -Wextra

# A Rule: How to make a .so from a .c file
lib%.so: %.c cache.h
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $<

main: $(OBJS)
	gcc -o $@ $(CFLAGS) $(OBJS) -ldl

# DEPENDENCIES

main.o: collatz.h cache.h

collatz.o: collatz.h

clean:
	rm -f main $(OBJS) $(LIBS)
