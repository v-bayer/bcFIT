# Makefile for client/server example
# see 'man make' for more details
SERVER          = server
CLIENT          = client
SERVER_SOURCES  = server.c
CLIENT_SOURCES  = client.c

DEFINES         = 

CFLAGS         = -std=c++98 -pedantic -Wextra -Wall
#CFLAGS		   = g++49 -std=c++11 -static-libstdc++

LIBRARIES       = #-llibrary_name

CC              = g++
SERVER_OBJECTS  = $(SERVER_SOURCES:.c=.o)
CLIENT_OBJECTS  = $(CLIENT_SOURCES:.c=.o)
INCLUDES        = #-I.
LIBDIRS         = 
LDFLAGS         = $(LIBDIRS) $(LIBRARIES)

###########################################

.SUFFIXES: .c .o

.c.o:
		$(CC) $(CFLAGS) -c $<

###########################################

all:		$(SERVER) $(CLIENT)

rebuild:	clean all

$(SERVER):	$(SERVER_OBJECTS)
		$(CC) $(SERVER_OBJECTS) $(LDFLAGS) -o $@

$(CLIENT):	$(CLIENT_OBJECTS)
		$(CC) $(CLIENT_OBJECTS) $(LDFLAGS) -o $@

###########################################

clean:
	rm -fr core* *~ $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(SERVER) $(CLIENT) .make.state .sb
