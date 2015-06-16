C=gcc
CC=clang
CFLAGS=-std=gnu99 -pedantic -Wall -Wextra -Werror
LFLAGS=-lrt -lpthread


PROGS=rivercrossing

all: $(PROGS)

rivercrossing: rivercrossing.c
	$(C) $(CFLAGS) rivercrossing.c -o rivercrossing $(LFLAGS)
