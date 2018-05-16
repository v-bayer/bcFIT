# Makefile for client/server example
# see 'man make' for more details
SERVER          = fakesrv
SERVER_SOURCES  = fakesrv.c

DEFINES         = 

CFLAGS         = -std=c++98 -pedantic -Wextra -Wall
#CFLAGS		   = g++49 -std=c++11 -static-libstdc++

LIBRARIES       = -lrt -lpthread

CC              = g++
SERVER_OBJECTS  = $(SERVER_SOURCES:.c=.o)
INCLUDES        = #-I.
LIBDIRS         = 
LDFLAGS         = $(LIBDIRS) $(LIBRARIES)

###########################################

.SUFFIXES: .c .o

.c.o:
		$(CC) $(CFLAGS) -c $<

###########################################

all:		$(SERVER)

rebuild:	clean all

$(SERVER):	$(SERVER_OBJECTS)
		$(CC) $(SERVER_OBJECTS) $(LDFLAGS) -lssh -o $@ 

###########################################

clean:
	rm -fr core* *~ $(SERVER_OBJECTS) $(SERVER) .make.state .sb

package:
	tar -pczf xbayer05.tar.gz fakesrv.cpp fakesrv.h test.sh Makefile manual.pdf README.txt