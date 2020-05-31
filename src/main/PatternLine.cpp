#include "../headers/PatternLine.h"

PatternLine::PatternLine() :
  length(DIMENSIONS),
  tiles(new PatternRow*[DIMENSIONS]()),
  printer(new Printer())
{
  for (int i = 0; i < DIMENSIONS; ++i) {
    tiles[i] = new PatternRow(PatternRow(i + 1, EMPTY));
  }
}

PatternLine::~PatternLine() {
  for (int i = 0; i < DIMENSIONS; ++i) {
    delete tiles[i];
  }

  delete [] tiles;
  delete printer;
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
    printer->error("Error: Attempting to access out of bounds Pattern Line row.");
  }

  return overflow;
}

void PatternLine::setColour(Colour colour, int row) {
  if (row >= 0 && row < DIMENSIONS) {
    tiles[row]->setColour(colour);
  } else {
    printer->error("Error: Attempting to access out of bounds Pattern Line row.");
  }
}

Colour PatternLine::getColour(int row) { 
  Colour colour = EMPTY;
  
  if (row >= 0 && row < DIMENSIONS) {
    colour = tiles[row]->getColour();
  } else {
    printer->error("Error: Attempting to access out of bounds Pattern Line row.");
  }

  return colour;
}

int PatternLine::getAmount(int row) {
  int amount = -1;

  if (row >= 0 && row < DIMENSIONS) {
    amount = tiles[row]->count();
  } else {
    printer->error("Error: Attempting to access out of bounds Pattern Line row.");
  }

  return amount;
}

void PatternLine::clearRow(int row) {
  if (row >= 0 && row < DIMENSIONS) {
    tiles[row]->clear();
  } else {
    printer->error("Error: Attempting to access out of bounds Pattern Line row.");
  }
}

bool PatternLine::isRowFull(int row) {
  bool isFull = true;
  
  if (row >= 0 && row < DIMENSIONS) {
    isFull = tiles[row]->isFull();
  } else {
    printer->error("Error: Attempting to access out of bounds Pattern Line row.");
  }

  return isFull;
}

void PatternLine::printPattern(int row) {
  if (row >= 0 && row < DIMENSIONS) {
    tiles[row]->printRow();
  } else {
    printer->error("Error: Attempting to access out of bounds Pattern Line row.");
  }
}

std::string PatternLine::toSaveString() {
  std::string output = "";

  for (int i = 0; i < DIMENSIONS; ++i) {
    output += tiles[i]->toSaveString();
  }

  return output;
}

PatternRow* PatternLine::getRow(int row) {
  PatternRow* result = nullptr;
  
  if (row >= 0 && row < DIMENSIONS) {
    result = tiles[row];
  } else {
    printer->error("Error: Attempting to access out of bounds Pattern Line row.");
  }

  return result;
}
