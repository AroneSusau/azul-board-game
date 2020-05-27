#ifndef MOSAIC_H
#define MOSAIC_H

#include <iostream>

#include "Bag.h"
#include "DiscardedLine.h"
#include "LinkedList.h"
#include "PatternLine.h"
#include "WallManager.h"
#include "Bag.h"
#include "LinkedList.h"

class Mosaic {
  public:
    Mosaic();
    ~Mosaic();

    // gets the wall manager
    WallManager* getWall();

    // gets the pattern line
    PatternLine* getPattern();

    // gets the discard line
    DiscardedLine* getDiscard();

    // places tiles onto pattern line, excess go to discard or box lid
    void place(Colour colour, int amount, int row, bool firstPlayer, Bag* bag);

    // returns true if the colour and row are valid inputs for the pattern line
    bool validatePatternLineInput(Colour colour, int row);

    // returns true if colour and row are valid inputs for the wall
    bool validateWallColourInput(Colour colour, int row);

    // prints out the mosaic
    void printMosaic(std::string player);

  private:
    WallManager* wall;
    PatternLine* pattern;
    DiscardedLine* discard;
};

#endif