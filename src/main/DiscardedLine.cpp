#include "../headers/DiscardedLine.h"

DiscardedLine::DiscardedLine() :
    amount(0),
    tiles(new Tile*[DISCARD_MAX]()),
    printer(new Printer())
{
    for (int i = 0; i < DISCARD_MAX; ++i) {
        tiles[i] = new Tile(EMPTY);
    }
}

DiscardedLine::~DiscardedLine() {
    for (int i = 0; i < DISCARD_MAX; ++i) {
        delete tiles[i];
    }

    delete [] tiles;
    delete printer;
}

bool DiscardedLine::add(Colour colour) {
    bool result = false;
    
    if (amount < DISCARD_MAX) {
        tiles[amount]->setColour(colour);
        ++amount;
        result = true;
    }

    return result;
}

Colour DiscardedLine::getColour(int index) {
  Colour colour = EMPTY;
  
  if (index >= 0 && index < DISCARD_MAX) {
      colour = tiles[index]->getColour();
  } else {
      printer->error("Error: Attempting to access out of bounds Discard tile.");
  }

  return colour;
}

void DiscardedLine::clear() {
    amount = 0;
    for (int i = 0; i < DISCARD_MAX; ++i) {
        tiles[i]->setColour(EMPTY);
    }
}

int DiscardedLine::count() {
    return amount;
}

int DiscardedLine::getMinusPoints() {
    int points[9] = {0, 1, 2, 4, 6, 8, 11, 14, 99};
    int result = 99;
    
    if (amount >= 0 || amount <= 7) {
        result = points[amount];
    }

    return result;
}

void DiscardedLine::setDiscard(Colour colour, int index) {
    if (index >= 0 && index < DISCARD_MAX) {
        tiles[index]->setColour(colour);
    } else {
        printer->error("Error: Attempting to access out of bounds Discard tile.");
    }
}

void DiscardedLine::printDiscard() {
    std::cout << "broken: ";

    for (int i = 0; i < DISCARD_MAX; ++i) {
        std::cout << (char) tiles[i]->getColour() << " ";
    }
}

std::string DiscardedLine::toSaveString() {
    std::string output = "";

    for (int i = 0; i < DISCARD_MAX; ++i) {
        output += tiles[i]->getColour();
    }

    return output;
}
