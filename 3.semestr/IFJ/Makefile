#Makefile pro interpreter pro jazyk IFJ14
#Date - 09/2014 - 12/2014
#Authors - Maros Janota, Matous Jezersky, Tomas Furch, Vaclav Bayer, Simon Hives 

CC=gcc
CFLAGS= -Wall -std=c99 -pedantic -Wextra -g

all: interpreter 
	
interpreter: scaner.o ial.o key_word_table.o syntax.o semantics.o precedence.o str.o i_list.o interpret.o
	$(CC) $(CFLAGS) -o interpreter scaner.o ial.o key_word_table.o syntax.o semantics.o precedence.o str.o i_list.o interpret.o
	
scaner.o: scaner.c scaner.h
	$(CC) $(CFLAGS) -c scaner.c
	
ial.o: ial.c ial.h
	$(CC) $(CFLAGS) -c ial.c
	
key_word_table.o: key_word_table.c key_word_table.h
	$(CC) $(CFLAGS) -c key_word_table.c
	
syntax.o: syntax.c syntax.h
	$(CC) $(CFLAGS) -c syntax.c
	
semantics.o: semantics.c semantics.h
	$(CC) $(CFLAGS) -c semantics.c
	
precedence.o: precedence.c precedence.h  
	$(CC) $(CFLAGS) -c precedence.c
	
str.o: str.c str.h
	$(CC) $(CFLAGS) -c str.c
	
i_list.o: i_list.c i_list.h
	$(CC) $(CFLAGS) -c i_list.c

interpret.o: interpret.c interpret.h
	$(CC) $(CFLAGS) -c interpret.c
	
stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c
	
.PHONY: clean
.PHONY: zip
	
clean:
	rm -f *.o
	rm -f interpreter
	
zip: *.c *.h Makefile 
	zip xjanot01 *.c *.h Makefile rozdeleni dokumentace.pdf rozsireni

