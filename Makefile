SHELL = /bin/sh
CC = gcc
CFLAGS = -Wall -pedantic -std=c99
LDFLAGS =  
VPATH=src

# Following makefile conventions:
# https://www.gnu.org/prep/standards/html_node/Makefile-Conventions.html
# Prefix for local installs.
DESTDIR = 

## 
##   VERIFY THE FOLLOWING VARIABLES:
##
# Executable location
prefix = $(DESTDIR)/usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin

# Man directory.
datarootdir = $(prefix)/share
mandir = $(datarootdir)/man
man1dir = $(mandir)/man1
man1ext = .1

# Executable Name
TARGET=dl


# Everything else is part of the makefile.

all: dl

debug: CFLAGS += -DDEBUG -g
debug: dl

clean: clean-build clean-install

clean-build:
	rm *.o
	rm $(TARGET)

clean-install:
	#rm Makefile # This is for configure

install: install-bin install-man

install-bin:
	mkdir -p $(bindir)
	cp $(TARGET) $(bindir)/$(TARGET)

install-man:
	mkdir -p $(man1dir)
	cp $(TARGET)$(man1ext) $(man1dir)/$(TARGET)$(man1ext)
	mandb

uninstall:
	rm $(bindir)/$(TARGET)
	rm $(man1dir)/$(TARGET)$(man1ext)

dl: dl.o
	$(CC) $(CFLAGS) dl.o -o $(TARGET)

dl.o: dl.c
