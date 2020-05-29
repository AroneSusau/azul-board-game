#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <iostream>

#include "Tile.h"
#include "Colour.h"
#include "Types.h"
#include "CentreFactory.h"
#include "Printer.h"
#include "Fonts.h"

class Factory {
public:
    Factory();
    ~Factory();

    // return a tile colour at the index position
    Colour get(int index);

    // updates colour at tile
    void update(int index, Colour colour);

    // removes all tiles of a specific colour
    void remove(Colour c);
    
    // returns the list of tiles to collect
    int count(Colour colour);

    // returns true if it contains the specified colour
    bool contains(Colour colour);

    // resets all tile colours
    void clear();

    // returns true if factory is empty
    bool isEmpty();

    // returns string details of colours
    std::string toString();

    // returns string details of colours in save format
    std::string toSaveString();

protected: 
    int length;
    Tile** tiles;
    Printer* printer;
};

#endif
