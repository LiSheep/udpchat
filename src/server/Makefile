VPATH = ../deps
CFLAGS = -Wall -ltalloc -lpthread -I../deps -L../deps -g 
CC = gcc
OBJECTS = client.o hash.o stack.o udputil.o threadutil.o msg.o merror.o

server:server.c $(OBJECTS)
	$(CC) $^ -o $@ $(CFLAGS) -L$(VPATH) -I$(VPATH)

.PHONY : clean
clean:
	rm -f *.o server	
