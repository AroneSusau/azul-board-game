#include "../headers/PatternRow.h"

PatternRow::PatternRow(): 
  colour(EMPTY),
  length(0),
  max(ROWS)
{}

PatternRow::PatternRow(int max, Colour colour): 
  colour(colour),
  length(0),
  max(max)
{}

PatternRow::~PatternRow() {}

Colour PatternRow::getColour() {
  return colour;
}

void PatternRow::setColour(Colour colour) {
  this->colour = colour;
}

int PatternRow::count() {
  return length;
}

void PatternRow::setCount(int count) {
  length = count;
}

bool PatternRow::add() {

  bool added = false;

  if (length < max) {
    ++length;
    added = true;
  }

  return added;
}

bool PatternRow::add(Colour colour) {

  bool added = false;

  if (length < max && this->colour == colour) {
    ++length;
    added = true;
  }

  return added;
}

bool PatternRow::remove() {
  
  bool removed = false;

  if (length > 0) {
    --length;
    removed = true;
  }

  return removed;
}

int PatternRow::getMax() {
  return max;
}

void PatternRow::setMax(int max) {
  this->max = max;
}

void PatternRow::clear() {
  length = 0;
  colour = EMPTY;
}

bool PatternRow::isEmpty() {
  return length == 0;
}

bool PatternRow::isFull() {
  return length == max;
}

void PatternRow::printRow() {
  for (int i = 4; i > max - 1; --i) {
    std::cout << "  ";
  }
  
  for (int i = max - 1; i >= 0; --i) {
    if (i >= length) {
      std::cout << (char) EMPTY << " ";
    } else {
      std::cout << (char) colour << " ";
    }
  }
}

std::string PatternRow::toSaveString() {
  std::string output = "";

  for (int i = max; i > 0; --i) {
    if (i > length) {
      output += (char) EMPTY;
    } else {
      output += (char) colour;
    }
  }

  return output;
}
