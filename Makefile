vpath %.cpp src/main

OBJECTS = main.o CentreFactory.o Loader.o Factory.o Azul.o PatternRow.o WallManager.o Player.o BaseEngine.o GameEngine.o Printer.o Saver.o Bag.o Tile.o Mosaic.o PatternLine.o DiscardedLine.o Wall.o LinkedList.o Node.o

.default: all

all: Azul

run: Azul
	./Azul

clean:
	rm Azul
	rm objs/*.o

test:
	bash scripts/run_tests.sh $v

Azul: $(OBJECTS)
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $(addprefix objs/, $(OBJECTS))

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^ -o objs/$@