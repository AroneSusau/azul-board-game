#include "../headers/Bag.h"

Bag::Bag():
  seed(0),
  tiles(new LinkedList()),
  lid(new std::vector<Tile*>()),
  printer(new Printer())
{}

Bag::Bag(int seed):
  seed(seed),
  tiles(new LinkedList()),
  lid(new std::vector<Tile*>()),
  printer(new Printer())
{}

Bag::~Bag() {

  delete tiles;
  
  for (int i = 0; i < (int) lid->size(); ++i) {
    delete lid->at(i);
  }

  delete lid;
  delete printer;

}

void Bag::shuffle() {

  Colour colours[5] = {
    RED,
    YELLOW,
    DARK_BLUE,
    LIGHT_BLUE,
    BLACK
  };

  Colour tmp [TILES_MAX] = {};

  for (int i = 0; i < TILES_MAX; ++i) {
    float res = floor(i / 20);
    tmp[i] = colours[(int) res % 5];
  }

  std::default_random_engine engine(seed);
  std::uniform_int_distribution<int> uniform_dist(TILES_MIN, TILES_MAX - 1);

  for (int i = 0; i < TILES_MAX; ++i) {
    tiles->addBack(tmp[uniform_dist(engine)]);
  }

}

int Bag::getSeed() {
  return seed;
}

void Bag::setSeed(int seed) {
  this->seed = seed;
}

void Bag::addTile(Colour colour) {
  
  Colour validTiles[DIMENSIONS] = {BLACK, RED, LIGHT_BLUE, DARK_BLUE, YELLOW};
  bool isValid = false;

  for (int i = 0; i < DIMENSIONS; ++i) {
    if (colour == validTiles[i]) {
      isValid = true;
    }
  }

  if (isValid) {
    tiles->addFront(colour);
  } else {
    printer->error("Error: Attempting to add invalid tile colour [" + std::string(1, (char) colour) + "] to bag.");
  }
}

void Bag::removeTile(int index) {
  tiles->remove(index);
}

Colour Bag::nextBagTile() {
  Colour colour = tiles->getHead()->getData()->getColour();
  tiles->remove(0);

  return colour;
}

Colour Bag::getBagTile(int index) {
  Colour result = EMPTY;
  
  if (index >= 0 && index < TILES_MAX) {
    result = tiles->get(index)->getData()->getColour();
  } else {
    printer->error("Error: Attepting to access out of bounds bag tile.");
  }

  return result;
}

void Bag::addLidTile(Colour colour) {
  Colour validTiles[DIMENSIONS] = {BLACK, RED, LIGHT_BLUE, DARK_BLUE, YELLOW};
  bool isValid = false;

  for (int i = 0; i < DIMENSIONS; ++i) {
    if (colour == validTiles[i]) {
      isValid = true;
    }
  }

  if (isValid) {
    lid->push_back(new Tile(colour));
  } else {
    printer->error("Error: Attempting to add invalid tile colour [" + std::string(1, (char) colour) + "] to Lid.");
  }
}

void Bag::removeLidTile(int index) {
  if (index >= 0 && index < (int)lid->size()) {
    lid->erase(lid->begin() + index);
  }
}

Colour Bag::getLidTile(int index) {
  Colour result = EMPTY;
  
  if (index >= 0 && index < TILES_MAX) {
    result = lid->at(index)->getColour();
  }

  return result;
}

void Bag::refillBag() {
  
  for (int i = 0; i < (int) lid->size(); ++i) {

    Colour colour = lid->at(i)->getColour();
    
    if (colour != EMPTY && colour != FIRST_PLAYER) {
      tiles->addBack(colour);
    }
  }

  resetLid();

}

void Bag::resetLid() {  
  while ((int) lid->size() > 0) {
    delete lid->at(0);
    lid->erase(lid->begin());
  }
}

std::string Bag::toBagSaveString() {
  std::string output = "";

  for (int i = 0; i < (int) tiles->size(); ++i) {
    output += tiles->get(i)->getData()->getColour();
  }

  return output;
}

std::string Bag::toLidSaveString() {
  std::string output = "";

  if ((int) lid->size() > 0) {
    for (int i = 0; i < (int) lid->size(); ++i) {
      output += lid->at(i)->getColour();
    }
  } else {
    output += "=";
  }

  return output;
}

void Bag::clear() {
  
  tiles->clear();
  lid->clear();

}
