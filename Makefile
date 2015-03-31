SHELL = /bin/sh
CC = gcc
CFLAGS = -ansi -Wall -pedantic -std=c99
LFLAGS =  


VPATH=src
TARGET=hr

# Need to follow better makefile conventions:
# https://www.gnu.org/prep/standards/html_node/Makefile-Conventions.html
DESTDIR = 
prefix = $(DESTDIR)/usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin

datarootdir = $(prefix)/share
mandir = $(datarootdir)/man
man1dir = $(mandir)/man1
man1ext = .1

all: hr.o

debug: CFLAGS += -DDEBUG -g
debug: hr.o

clean: clean-build clean-install

clean-build:
	rm *.o
	rm $(TARGET)

clean-install:
	#rm Makefile # This is for configure

install:
	cp $(TARGET) $(bindir)/$(TARGET)

install-man:
	mkdir -p $(man1dir)
	cp $(TARGET)$(man1ext) $(man1dir)/$(TARGET)$(man1ext)
	mandb

uninstall:
	rm $(bindir)/$(TARGET)
	rm $(man1dir)/$(TARGET)$(man1ext)

hr: hr.o
	$(CC) $(CFLAGS) hr.o -o $(TARGET)

hr.o: hr.c
