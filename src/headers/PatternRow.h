#ifndef PATTERN_ROW_H
#define PATTERN_ROW_H

#include <iostream>
#include <string>

#include "Colour.h"
#include "Types.h"
#include "Fonts.h"

class PatternRow {

  public:

    PatternRow();
    PatternRow(int max, Colour colour);
    ~PatternRow();

    // Returns colour of row
    Colour getColour();
    
    // Sets the colour of the pattern
    void setColour(Colour colour);

    // Returns how many tiles are in the row
    int count();

    // Sets count to new value
    void setCount(int count);

    // Returns true if successfully added to count
    bool add();

    // Returns true if successfully added to count and the colour matches the row colour
    bool add(Colour colour);

    // Returns true if successfully removed from count
    bool remove();

    // Resets the row
    void clear();

    // Returns what the max length of the row is
    int getMax();

    // Sets max to new value
    void setMax(int max);

    // Returns true if the row is empty
    bool isEmpty();

    // Returns true if the row is full
    bool isFull();

    // Prints the row in a neat format
    void printRow();

    // Prints the row in a neat format
    std::string toSaveString();

  private:

    // Colour in row
    Colour colour;

    // Current amount of tiles in row
    int length;

    // Maximum tiles permitted in row
    int max;

};

#endif