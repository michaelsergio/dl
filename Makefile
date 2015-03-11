CFLAGS = -ansi -Wall -pedantic -std=c99
LFLAGS =  
CC = gcc

all: hr.o

debug: CFLAGS += -DDEBUG -g
debug: hr.o

hr.o:
	$(CC) $(CFLAGS) hr.c -o hr.exe

clean:
	rm hr.exe
