VPATH = ../deps
CFLAGS = -Wall -g -ltalloc -lpthread -g
CC = gcc
OBJECTS = client.o hash.o stack.o udputil.o threadutil.o msg.o merror.o

server:server.o $(OBJECTS)
	$(CC) -o $@ $? $(CFLAGS)
server.o:server.c
	$(CC) -c $? 

.PHONY : clean
clean:
	rm -f *.o server	
