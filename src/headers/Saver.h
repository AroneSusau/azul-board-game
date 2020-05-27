#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include <iostream>
#include <fstream>
#include <vector>

#include "BaseEngine.h"

class Saver {

  public:
    Saver();
    ~Saver();

    // Saves the state of the game engine object in a file from the specified name
    void save(std::string name, BaseEngine* gameEngine);

  private:
    
    // Writes the state of the player to the given file
    void savePlayer(std::ofstream* file, BaseEngine* gameEngine);
    
    // Writes the state of the factory to the given file
    void saveFactory(std::ofstream* file, BaseEngine* gameEngine);
    
    // Writes the state of the game to the given file
    void saveGame(std::ofstream* file, BaseEngine* gameEngine);

};

#endif
