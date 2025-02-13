
OBJS = main.o cache.o collatz.o
LIBS = libbozo_cache.so librex_cache.so libmemoize.so

all: main $(LIBS)

CC = gcc
CFLAGS = -Wall -ansi -pedantic -Wno-comment -std=c11 -Wextra

# A Rule: How to make a .so from a .c file
lib%.so: %.c
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $<

main: $(OBJS)
	gcc -o $@ $(CFLAGS) $(OBJS) -ldl

# DEPENDENCIES
# Note that you do not have to list that "x.o" depends on "x.c"

main.o: collatz.h cache.h

collatz.o: collatz.h

cache.o: cache.h

libbozo_cache.so: cache.h

libmemoize.so: cache.h

librex_cache.so: cache.h

clean:
	rm -f main $(OBJS) $(LIBS)
