#ifndef WALL_H
#define WALL_H

#include <iostream>

#include "Tile.h"
#include "Colour.h"
#include "Types.h"

template <typename T>
class Wall {
  public:
    Wall<T>();
    ~Wall<T>();

    // Returns tile at given row and column
    T* get(int row, int col);

    // Sets the tile at row and column the specified value
    void set(int row, int col, T* value);

  private:
    T*** tiles;
};

#endif