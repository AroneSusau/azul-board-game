OS = $(shell uname)

SOURCEDIR = src
BUILDDIR = objs
VPATH =  src/main : objs/

ifeq ($(OS), Linux)
  VER=c++1y
else
  VER=c++14
endif

CC = g++
CFLAGS = -Wall -Werror -std=$(VER)

BASE_OBJECTS = main.o CentreFactory.o Validator.o Loader.o Factory.o Azul.o PatternRow.o WallManager.o Player.o BaseEngine.o GameEngine.o Printer.o Saver.o Bag.o Tile.o Mosaic.o PatternLine.o DiscardedLine.o Wall.o LinkedList.o Node.o
OBJECTS = $(addprefix objs/, $(BASE_OBJECTS))

.default: all

all: Azul

test:
	bash ./scripts/run_tests.sh

clean:
	if [ $(shell ls objs | grep -wv "*.o" -c) -gt 0 ]; then rm $(BUILDDIR)/*.o; fi;
	if [ -f Azul ]; then rm Azul; fi;

Azul: $(BASE_OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o Azul

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ -o $(BUILDDIR)/$@
	