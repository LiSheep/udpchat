VPATH = ../deps
LUAHOME = /usr/local/lua51
CC = gcc
OBJECTS = merror.o udputil.o

CFLAGS = -llua -lm -ldl -fPIC -shared -Wall -L $(LUAHOME)/lib -I $(LUAHOME)/include -I $(VPATH)

.PHONY : client
client:myudp.so
	
myudp.so:myudp.c $(OBJECTS)
	$(CC) -o $@ $? $(CFLAGS)
	cp $(VPATH)/cjson.so $(VPATH)/thread.so .

merror.o:merror.c
	gcc -c $?
udputil.o:udputil.c 
	gcc -c $?
.PHONY : clean
clean:
	rm -f *.o *.so

