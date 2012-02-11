##############################################################################
# 
# Makefile - CS248 Assignment 2, Winter 2010
#
# Contents: add to include and linking paths the class directory for SDL
#           library support.
#
##############################################################################

prefix = /afs/ir/class/cs248
sdlcommand := $(prefix)/bin/sdl-config

CXX       := g++
CXXFLAGS  := -g -Wall -c \
	     $(shell ${sdlcommand} --cflags)
LDFLAGS   := -g -Wall \
	     $(shell ${sdlcommand} --libs) -lSDL_image
RM        := rm -f
EXEC      := run

all: $(EXEC)

$(EXEC): main.o minigl.o mglmesh.o
	$(CXX) $(LDFLAGS) $^ -o $@

main.o: main.cpp minigl.h mglmesh.h
	$(CXX) $(CXXFLAGS) $< -o $@

minigl.o: minigl.cpp minigl.h
	$(CXX) $(CXXFLAGS) $< -o $@

mglmesh.o: mglmesh.cpp mglmesh.h minigl.h
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	$(RM) *~ *.bak *.o $(EXEC)
