#ifndef PATTERN_LINE_H
#define PATTERN_LINE_H

#include <iostream>

#include "Colour.h"
#include "Tile.h"
#include "PatternRow.h"
#include "Types.h"
#include "Printer.h"

class PatternLine {
  public:
    PatternLine();
    ~PatternLine();

    // adds tiles to a row, if the row has not set colour it sets the colour
    // if the colour does not match the row colour returns -1
    // returns the number of tiles that would be broken
    int add(Colour colour, int amount, int row);

    // sets the colour of row
    void setColour(Colour colour, int row);

    // gets the colour of row
    Colour getColour(int row);

    // returns the amount of tiles that are already in the row
    int getAmount(int row);

    // clears the specifed row and sets all values to empty
    void clearRow(int row);

    // checks if a row is full or not
    bool isRowFull(int row);

    // prints out a row
    void printPattern(int row);

    // returns the size of the array
    int size();

    // prints out the pattern line in save string format
    std::string toSaveString();

  private:
    int length;
    PatternRow** tiles;
    Printer* printer;
};

#endif