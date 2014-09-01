SRCDIR = ../src
BINDIR = ../bin
CC = g++
DEBUG = -ggdb3
DEBUGGER = gdb
CFLAGS = -Wall `sdl2-config --cflags`
LFLAGS = -Wall `sdl2-config --libs`

SOURCES=$(SRCDIR)/main.cpp $(SRCDIR)/game.cpp $(SRCDIR)/graphics.cpp $(SRCDIR)/model.cpp $(SRCDIR)/units.cpp

build:
	$(CC) $(CFLAGS) $(LFLAGS) $(SOURCES) -o $(BINDIR)/pc.out

install: build
	$(BINDIR)/pc.out

run:
	$(CC) $(CFLAGS) $(LFLAGS) $(SOURCES) -o $(BINDIR)/pc.out && $(BINDIR)/pc.out

debug:
	$(CC) $(DEBUG) $(CFLAGS) $(LFLAGS) $(SOURCES) -o $(BINDIR)/pc.out && $(DEBUGGER) $(BINDIR)/pc.out

