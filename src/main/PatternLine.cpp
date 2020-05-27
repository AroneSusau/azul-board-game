#include "../headers/PatternLine.h"

PatternLine::PatternLine() :
  length(DIMENSIONS),
  tiles(new PatternRow*[DIMENSIONS]())
{
  for (int i = 0; i < DIMENSIONS; ++i) {
    tiles[i] = new PatternRow(PatternRow(i + 1, EMPTY));
  }
}

PatternLine::~PatternLine() {
  for (int i = 0; i < DIMENSIONS; ++i) {
    delete tiles[i];
  }
}

int PatternLine::add(Colour colour, int amount, int row) {
  int overflow = -1;

  if (row >= 0 && row < DIMENSIONS) {
    if (tiles[row]->getColour() == EMPTY || tiles[row]->getColour() == colour) {
  
      tiles[row]->setColour(colour);
      overflow = 0;

      for (int i = 0; i < amount; ++i) {
        if (!tiles[row]->add()) {
          ++overflow;
        }
      }
    }
  } else {
    std::cout << "Error: Attempting to access out of bounds Pattern Line row." << std::endl;
  }

  return overflow;
}

void PatternLine::setColour(Colour colour, int row) {
  if (row >= 0 && row < DIMENSIONS) {
    tiles[row]->setColour(colour);
  } else {
    std::cout << "Error: Attempting to access out of bounds Pattern Line row." << std::endl;
  }
}

Colour PatternLine::getColour(int row) { 
  Colour colour = EMPTY;
  
  if (row >= 0 && row < DIMENSIONS) {
    colour = tiles[row]->getColour();
  } else {
    std::cout << "Error: Attempting to access out of bounds Pattern Line row." << std::endl;
  }

  return colour;
}

int PatternLine::getAmount(int row) {
  int amount = -1;

  if (row >= 0 && row < DIMENSIONS) {
    amount = tiles[row]->count();
  } else {
    std::cout << "Error: Attempting to access out of bounds Pattern Line row." << std::endl;
  }

  return amount;
}

void PatternLine::clearRow(int row) {
  if (row >= 0 && row < DIMENSIONS) {
    tiles[row]->clear();
  } else {
    std::cout << "Error: Attempting to access out of bounds Pattern Line row." << std::endl;
  }
}

bool PatternLine::isRowFull(int row) {
  bool isFull = true;
  
  if (row >= 0 && row < DIMENSIONS) {
    isFull = tiles[row]->isFull();
  } else {
    std::cout << "Error: Attempting to access out of bounds Pattern Line row." << std::endl;
  }

  return isFull;
}

void PatternLine::printPattern(int row) {
  if (row >= 0 && row < DIMENSIONS) {
    tiles[row]->printRow();
  } else {
    std::cout << "Error: Attempting to access out of bounds Pattern Line row." << std::endl;
  }
}

std::string PatternLine::toSaveString() {
  std::string output = "";

  for (int i = 0; i < DIMENSIONS; ++i) {
    output += tiles[i]->toSaveString();
  }

  return output;
}
