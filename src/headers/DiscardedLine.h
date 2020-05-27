#ifndef DISCARDED_LINE_H
#define DISCARDED_LINE_H

#include <iostream>

#include "Tile.h"
#include "Types.h"

class DiscardedLine {
public:
    DiscardedLine();
    ~DiscardedLine();

    // gets colours at specifed index on discard line
    Colour getColour(int index);

    // returns true if able to add tile to discard line of specified colour
    bool add(Colour colour);
    
    // empties out the discard line
    void clear();

    // Minus points are determined by the amount of non-empty tiles in the discard
    int getMinusPoints();
    
    // returns number of tiles on discard line
    int count();

    // sets colour of discard line at specified index
    void setDiscard(Colour colour, int index);

    // prints out the discard line
    void printDiscard();

    // returns string of save file format for discard line
    std::string toSaveString();

private:
    
    // number of tiles in discard line
    int amount;

    // discard line tiles
    Tile** tiles;
};

#endif