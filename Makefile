CFLAGS = -ansi -Wall -pedantic -std=c99
LFLAGS =  
CC = gcc
MANPATH=/usr/local/share/man

all: hr.o

debug: CFLAGS += -DDEBUG -g
debug: hr.o

hr.o:
	$(CC) $(CFLAGS) hr.c -o hr.exe

clean:
	rm hr.exe

install-man:
	cp $(MANFILE) $(MANPATH)/$(MANFILE)
	sudo mandb
