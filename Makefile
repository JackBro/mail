CC = g++
CFLAGS=-Wall -ggdb -g3 -pthread 
LDFLAGS= -lcurl

OBJECTS=mchain.o loader.o trainer.o getter.o 
SOURCE=mchain.cpp loader.cpp trainer.cpp getter.cpp 

all:  mchain

mchain: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o mchain $(LDFLAGS)



clean:
	rm -rf *.o mchain
#	find /tmp/* -maxdepth 0 -name 'led*' -exec rm '{}' ';'

