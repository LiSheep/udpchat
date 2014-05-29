VPATH = ../deps
LUAHOME = /usr/local/lua51
CFLAGS = -Wall -ltalloc -lpthread -g -llua -lm -ldl -I../deps -L$(VPATH) -L$(LUAHOME)/lib -I$(LUAHOME)/include -Wl,-rpath=$(LUAHOME)/lib  -std=gnu99
CC = gcc
OBJECTS = client.o hash.o stack.o udputil.o threadutil.o msg.o merror.o

server:server.c $(OBJECTS)
	$(CC) $^ -o $@ $(CFLAGS) -L$(VPATH) -I$(VPATH)

.PHONY : clean
clean:
	rm -f *.o server
