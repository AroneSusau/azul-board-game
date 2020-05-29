#include "../headers/Factory.h"

Factory::Factory() : 
  length(FACTORIES_SIZE),
  tiles(new Tile*[FACTORIES_SIZE]),
  printer(new Printer())
{
  for (int i = 0; i < FACTORIES_SIZE; ++i) {
    tiles[i] = new Tile(EMPTY);
  }
}

Factory::~Factory() {
  
  for (int i = 0; i < FACTORIES_SIZE; ++i) {
    delete tiles[i];
  }
  
  delete [] tiles;
  delete printer;
}

Colour Factory::get(int index) { 
  return tiles[index]->getColour(); 
}

void Factory::update(int index, Colour colour) {
  if (index >= 0 && index < FACTORIES_SIZE) {
    tiles[index]->setColour(colour);
  } else {
    printer->error("Error: Attempting to access out of bounds factory tile.");
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
    
    Colour colour = tiles[i]->getColour();

    if (colour == RED) {
      output += BG_RED;
    } else if (colour == YELLOW) {
      output += BG_YELLOW;
      output += C_BLACK;
    }  else if (colour == LIGHT_BLUE) {
      output += BG_CYAN;
    }  else if (colour == DARK_BLUE) {
      output += BG_BLUE;
    }  else if (colour == BLACK) {
      output += BG_BLACK;
    }  else if (colour == EMPTY) {
      output += BG_WHITE;
    }
    
    output += " ";
    output += (char) colour;
    output += " ";
    output += C_RESET;
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
