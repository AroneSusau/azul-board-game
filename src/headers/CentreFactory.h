#ifndef CENTRE_FACTORY_H
#define CENTRE_FACTORY_H

#include <string>
#include <vector>
#include <iostream>

#include "Types.h"
#include "Tile.h"
#include "Printer.h"

class CentreFactory {
  public:
    CentreFactory();
    ~CentreFactory();

    // return a tile colour at the index position
    Colour get(int index);

    // return a tile colour at the index position
    void add(Colour colour);

    // updates colour at tile
    void update(int index, Colour colour);

    // removes all tiles of a specific colour
    void remove(Colour colour);

    // removes all tiles of a specific colour
    void remove(int index);
    
    // returns the number of tiles of the specified colour
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

    // sets player token flag
    void setToken(bool token);

    // returns the first player token
    bool getToken();

    // returns the size of the tiles list
    int size();

  private:
    std::vector<Tile*>* tiles;
    bool hasToken;
    Printer* printer;
};

#endif