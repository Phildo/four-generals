SRCDIR = ../src
BINDIR = ../bin
CC = g++
DEBUG = -ggdb3
DEBUGGER = gdb
CFLAGS = -Wall `sdl2-config --cflags`
LFLAGS = -Wall `sdl2-config --libs`

build:
	$(CC) $(CFLAGS) $(SRCDIR)/test_main.cpp $(SRCDIR)/model.cpp $(SRCDIR)/logger.cpp $(SRCDIR)/units.cpp -o $(BINDIR)/test.out && $(BINDIR)/test.out

install: build
	

run:
	$(CC) $(CFLAGS) $(SRCDIR)/test_main.cpp $(SRCDIR)/model.cpp $(SRCDIR)/units.cpp -o $(BINDIR)/test.out && $(BINDIR)/test.out

debug:
	$(CC) $(DEBUG) $(CFLAGS) $(SRCDIR)/test_main.cpp $(SRCDIR)/model.cpp $(SRCDIR)/units.cpp -o $(BINDIR)/test.out && $(DEBUGGER) $(BINDIR)/test.out

