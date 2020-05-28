#ifndef WALL_MANAGER_H
#define WALL_MANAGER_H

#include <iostream>

#include "Colour.h"
#include "Tile.h"
#include "Types.h"
#include "Wall.h"
#include "Fonts.h"
#include "Printer.h"

class WallManager {
  public:
    WallManager();
    ~WallManager();

    // Checks if (row, col) is true or false.
    bool isTileSet(int row, int col);

    // Checks if (row, col) is true or false for a specific colour.
    bool isColourSet(Colour colour, int row);

    // Returns index of colour on specified row
    int getColourDim(Colour colour, int row);

    // Returns the amount of points you get from placing a tile
    int roundPoints(int row, int col);

    // Gets the amount of true tiles
    int wallPoints();

    // Checks if the entire row is true
    bool rowTrue(int row);

    // Checks if the entire column is true
    bool colTrue(int col);

    // Returns true if all fives spots for a colour are true
    bool colourTrue(Colour colour);

    // Gets the amount of points at the end of the game
    int endPoints();

    // Returns the layout of tiles added
    Wall<bool>* getWall();

    // Returns the layout of tiles added
    Wall<Colour>* getColours();

    // Prints the walls tiles and colours out
    void printWall(int row);

    // Returns the state of the wall manager as a formatted string
    std::string toSaveString();

  private:
    // Used to set the pattern of colours on the wall;
    Colour setWallTile(int colour);

    Wall<bool>* wall;
    Wall<Colour>* colours;
    Printer* printer;
};

#endif