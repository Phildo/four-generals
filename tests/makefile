SRCDIR = ../src
BINDIR = bin
CC = g++
DEBUG = -ggdb3
DEBUGGER = gdb -q
CFLAGS = -Wall -std=c++11 `sdl2-config --cflags`
LFLAGS = -Wall `sdl2-config --libs`

server:
	$(CC) $(DEBUG) $(CFLAGS) $(LFLAGS) server_main.cpp $(SRCDIR)/server.cpp $(SRCDIR)/network.cpp $(SRCDIR)/string.cpp -o $(BINDIR)/server_test.out && $(DEBUGGER) $(BINDIR)/server_test.out

client:
	$(CC) $(DEBUG) $(CFLAGS) $(LFLAGS) client_main.cpp $(SRCDIR)/client.cpp $(SRCDIR)/network.cpp $(SRCDIR)/string.cpp -o $(BINDIR)/client_test.out && $(DEBUGGER) $(BINDIR)/client_test.out

