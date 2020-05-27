#include "../headers/Factory.h"

Factory::Factory() : 
  length(FACTORIES_SIZE),
  tiles(new Tile*[FACTORIES_SIZE])
{
  for (int i = 0; i < FACTORIES_SIZE; ++i) {
    tiles[i] = new Tile(EMPTY);
  }
}

Factory::~Factory() {}

Colour Factory::get(int index) { 
  return tiles[index]->getColour(); 
}

void Factory::update(int index, Colour colour) {
  if (index >= 0 && index < FACTORIES_SIZE) {
    tiles[index]->setColour(colour);
  } else {
    std::cout << "Error: Attempting to access out of bounds factory tile." << std::endl;
  }
}

void Factory::remove(Colour colour) {
  for (int i = 0; i < FACTORIES_SIZE; ++i) {
    if (tiles[i]->getColour() == colour) {
      tiles[i]->setColour(EMPTY);
    }
  }
}

int Factory::count(Colour colour) {
  int amount = 0;

  for (int i = 0; i < FACTORIES_SIZE; ++i) {
    if (tiles[i]->getColour() == colour) {
      ++amount;
    }
  }

  return amount;
}

void Factory::clear() {
  for (int i = 0; i < FACTORIES_SIZE; ++i) {
    tiles[i]->setColour(EMPTY);
  }
}

bool Factory::contains(Colour colour) {
  bool exists = false;
  
  for (int i = 0; i < FACTORIES_SIZE; ++i) {
    if (tiles[i]->getColour() == colour) {
      exists = true;
    }
  }

  return exists;
}

bool Factory::isEmpty() {
  bool empty = true;

  for(int i = 0; i < FACTORIES_SIZE; ++i) {
    if(tiles[i]->getColour() != EMPTY) {
      empty = false;
    }
  }

  return empty;
}

std::string Factory::toString() {
  std::string output = "";

  for(int i = 0; i < FACTORIES_SIZE; ++i) {
    output += (char) tiles[i]->getColour();
    output += " ";
  }

  return output;
}

std::string Factory::toSaveString() {
  std::string output = "";

  for (int i = 0; i < FACTORIES_SIZE; ++i) {
    output += (char) tiles[i]->getColour();
  }

  return output;
}
