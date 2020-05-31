#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

#include "LinkedList.h"
#include "Mosaic.h"

class Player {
public:
    
    Player();
    Player(int id, std::string name, int score, bool starter, bool isHuman);
    ~Player();

    // Sets the id of the player 
    void setId(int id);

    // Returns the player's ID
    int getId();

    // Sets the name of the player
    void setName(std::string name);

    // Retuns the player's name
    std::string getName();

    // Sets the score of the player
    void setScore(int score);

    // Adds to the score of the player
    void addScore(int points);

    // Returns the player's score
    int getScore();

    // Returns a reference to the player's mosaic board
    Mosaic* getMosaic();

    // Returns true if the player is holding the first player token
    bool getStarter();

    // Allows to set if the player is holding the first player token or not
    void setStarter(bool starter);

    // Returns flag for ai or human
    bool getIsHuman();

    // Set flag for ai or human
    void setIsHuman(bool human);

private:
    int id;
    std::string name;
    int score;
    bool starter;
    bool isHuman;
    Mosaic* mosaic;

};

#endif