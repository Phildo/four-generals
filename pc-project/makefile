SRCDIR = ../src
BINDIR = ../bin
CC = g++
DEBUG = -g
DEBUGGER = gdb
CFLAGS = -Wall -std=c++11 `sdl2-config --cflags`
LFLAGS = -Wall `sdl2-config --libs`

SOURCES=$(SRCDIR)/main.cpp $(SRCDIR)/game.cpp $(SRCDIR)/graphics.cpp $(SRCDIR)/input.cpp $(SRCDIR)/ui.cpp $(SRCDIR)/network.cpp $(SRCDIR)/server.cpp $(SRCDIR)/client.cpp $(SRCDIR)/connection.cpp $(SRCDIR)/event.cpp $(SRCDIR)/scene.cpp $(SRCDIR)/intro_scene.cpp $(SRCDIR)/host_scene.cpp $(SRCDIR)/join_scene.cpp $(SRCDIR)/room_scene.cpp $(SRCDIR)/sprite.cpp $(SRCDIR)/client_model.cpp $(SRCDIR)/server_model.cpp $(SRCDIR)/general.cpp $(SRCDIR)/string.cpp

build:
	$(CC) $(CFLAGS) $(LFLAGS) $(SOURCES) -o $(BINDIR)/pc.out

install: build
	$(BINDIR)/pc.out

run:
	$(CC) $(CFLAGS) $(LFLAGS) $(SOURCES) -o $(BINDIR)/pc.out && $(BINDIR)/pc.out

debug:
	$(CC) $(DEBUG) $(CFLAGS) $(LFLAGS) $(SOURCES) -o $(BINDIR)/pc.out && $(DEBUGGER) $(BINDIR)/pc.out

