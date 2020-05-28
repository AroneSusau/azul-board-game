#ifndef BAG_H
#define BAG_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>

#include "Tile.h"
#include "Types.h"
#include "LinkedList.h"
#include "Printer.h"

class Bag {

  public:
    Bag();
    Bag(int seed);
    ~Bag();

    // creates a random assortment of 100 tiles
    void shuffle();

    // gets the game seed
    int getSeed();

    // returns the game seed
    void setSeed(int seed);

    // adds a tile to the game bag
    void addTile(Colour colour);

    // removes a tile from the game bag at the specified index
    void removeTile(int index);

    // returns the colour of the next tile and removes it from the bag
    Colour nextBagTile();

    // returns the tile from the bag at the specified index
    Colour getBagTile(int index);

    // adds a tile to the lid of the specified colour
    void addLidTile(Colour tile);

    // removes a tile from the lid at the specified index
    void removeLidTile(int index);

    // returns the colour of a tile at the specified index
    Colour getLidTile(int index);

    // transfers all tiles from the lid to the bag
    void refillBag();

    // empties out the lid
    void resetLid();

    // clears all tiles in the bag and lid
    void clear();

    // prints the bag tiles out in save string format
    std::string toBagSaveString();

    // prints the lid tiles out in save string format
    std::string toLidSaveString();

    private:
      int seed;
      LinkedList* tiles;
      std::vector<Tile*>* lid;
      Printer* printer;

};

#endif
