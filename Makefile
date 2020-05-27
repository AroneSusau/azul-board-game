SOURCEDIR = src
BUILDDIR = objs
VPATH = $(shell find src -type d | tr '\n' ':' | sed 's/:/ : /g')

SOURCES = $(wildcard $(SOURCEDIR)/**/*.cpp)
BASE_OBJECTS = main.o CentreFactory.o Loader.o Factory.o Azul.o PatternRow.o WallManager.o Player.o BaseEngine.o GameEngine.o Printer.o Saver.o Bag.o Tile.o Mosaic.o PatternLine.o DiscardedLine.o Wall.o LinkedList.o Node.o
OBJECTS = $(addprefix objs/, $(BASE_OBJECTS))

.default: all

all: Azul

clean:
	if [ $(shell ls objs | grep -wv "*.o" -c) -gt 0 ]; then rm objs/*.o; fi;
	if [ -f Azul ]; then rm Azul; fi;

Azul: $(BASE_OBJECTS)
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $(OBJECTS)

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^ -o objs/$@