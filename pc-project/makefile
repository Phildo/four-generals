SRCDIR = ../src
BINDIR = bin
CC = g++
DEBUG = -g
DEBUGGER = gdb -q
CFLAGS = -Wall -std=c++11 `sdl2-config --cflags`
LFLAGS = -Wall `sdl2-config --libs` -lSDL2_image

SOURCES=$(SRCDIR)/main.cpp $(SRCDIR)/logger.cpp $(SRCDIR)/debug_list.cpp $(SRCDIR)/game.cpp $(SRCDIR)/graphics.cpp $(SRCDIR)/lerp.cpp $(SRCDIR)/lerp_rect.cpp $(SRCDIR)/particles.cpp $(SRCDIR)/input.cpp $(SRCDIR)/ui.cpp $(SRCDIR)/network.cpp $(SRCDIR)/server.cpp $(SRCDIR)/client.cpp $(SRCDIR)/event.cpp $(SRCDIR)/turn.cpp $(SRCDIR)/action.cpp $(SRCDIR)/victory_record.cpp $(SRCDIR)/scene.cpp $(SRCDIR)/intro_scene.cpp $(SRCDIR)/host_scene.cpp $(SRCDIR)/join_scene.cpp $(SRCDIR)/room_scene.cpp $(SRCDIR)/play_scene.cpp $(SRCDIR)/day_picker.cpp $(SRCDIR)/turn_picker.cpp $(SRCDIR)/show_turn_picker.cpp $(SRCDIR)/browse_turn_picker.cpp $(SRCDIR)/specify_turn_picker.cpp $(SRCDIR)/messager.cpp $(SRCDIR)/sprite.cpp $(SRCDIR)/model.cpp $(SRCDIR)/compass.cpp $(SRCDIR)/week.cpp $(SRCDIR)/conids.cpp $(SRCDIR)/client_model.cpp $(SRCDIR)/server_model.cpp $(SRCDIR)/general.cpp $(SRCDIR)/string.cpp

build:
	$(CC) $(CFLAGS) $(LFLAGS) $(SOURCES) -o $(BINDIR)/pc.out

install: build
	$(BINDIR)/pc.out

run: build
	$(BINDIR)/pc.out

run4:
	./run4.sh

debug:
	$(CC) $(DEBUG) $(CFLAGS) $(LFLAGS) $(SOURCES) -o $(BINDIR)/pc.out && $(DEBUGGER) $(BINDIR)/pc.out

debug4:
	./debug4.sh

kill:
	killAll pc.out
