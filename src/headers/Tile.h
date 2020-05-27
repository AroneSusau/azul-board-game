#ifndef TILE_H
#define TILE_H

#include "Colour.h"

class Tile {
  
  public:
    Tile();
    ~Tile();
    Tile(Colour colour);

    // Returns the colour of the tile
    Colour getColour();

    // Sets the colour of this tile to the specified colour
    void setColour(Colour colour);

  private:
    Colour colour;
  
};

#endif
