#include "../headers/Mosaic.h"

Mosaic::Mosaic() :
  wall(new WallManager()),
  pattern(new PatternLine()),
  discard(new DiscardedLine())
{}

Mosaic::~Mosaic() {
  delete wall;
  delete pattern;
  delete discard;
}

WallManager* Mosaic::getWall() { 
  return wall; 
}

PatternLine* Mosaic::getPattern() { 
  return pattern; 
}

DiscardedLine* Mosaic::getDiscard() {
  return discard; 
}

void Mosaic::place(Colour colour, int amount, int row, bool firstPlayer, Bag* bag) {

  bool discardNotFull = true;
  int overflow = pattern->add(colour, amount, row);

  if (firstPlayer) {
    discardNotFull = discard->add(FIRST_PLAYER);
    firstPlayer = false;
  }

  while (overflow > 0) {
    if (discardNotFull) {

      discardNotFull = discard->add(colour);

    } else {
      bag->addLidTile(colour);
    }

    --overflow;
  }
}

bool Mosaic::validatePatternLineInput(Colour colour, int row) {
  
  bool valid = true;

  if (row >= 0 && row < DIMENSIONS) {
    
    Colour rowColour = (Colour) pattern->getColour(row);

    bool notMatching = colour != rowColour;
    bool notEmpty = rowColour != EMPTY;
    bool lineFull = pattern->isRowFull(row);

    if (notMatching && notEmpty) {
      valid = false;
      
      std::cout 
        << "Error: Input colour is " 
        << (char) colour << " but must be " 
        << (char) rowColour << " instead." << std::endl;
    
    } else if (lineFull) {

      valid = false;
      std::cout << "Error: Pattern Line " << row + 1 << " is already full." << std::endl;

    }
  } else {
    valid = false;
  }

  return valid;
}

bool Mosaic::validateWallColourInput(Colour colour, int row) {
  bool valid = true;

  if (row >= 0 && row < DIMENSIONS) {
    for (int i = 0; i < DIMENSIONS; ++i) {
      bool positionFilled = wall->isTileSet(row, i);
      Colour wallColour = (Colour) *wall->getColours()->get(row, i);

      if (positionFilled && wallColour == colour) {
        valid = false;

        std::cout 
          << "Error: Input colour " << (char) colour 
          << " has already been filled on the wall."  << std::endl;
      }
    }
  } else {
    valid = false;
  }

  return valid;
}

void Mosaic::printMosaic(std::string player) {
  std::cout << "Mosaic for " << player.c_str() << std::endl;
  
  for (int i = 0; i < 5; ++i) {
    std::cout << i + 1 << ": ";
    pattern->printPattern(i);
    std::cout << "|| ";
    wall->printWall(i);
  }
  
  discard->printDiscard();
}
