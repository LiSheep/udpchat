CFLAGS = -Wall -g -ltalloc -lpthread
objects = client.o hash.o stack.o udputil.o threadutil.o merror.o msg.o server.o
STD=-std=c99

server:$(objects)
	gcc -o server $(objects) $(CFLAGS) $(STD)
server.o:client.h hash.h stack.h udputil.h threadutil.h msg.h
msg.o:client.h
threadutil.o:merror.h
udputil.o:merror.h

.PHONY : clean
clean:
	rm -f *.o server
