
#include "../headers/Tile.h"

Tile::Tile():
  colour(EMPTY)
{}

Tile::~Tile() {}

Tile::Tile(Colour colour):
  colour(colour)
{}

Colour Tile::getColour() {
  return colour;
}

void Tile::setColour(Colour colour) {
  this->colour = colour;
}