#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <fstream>
#include <vector>

#include "Player.h"
#include "Factory.h"
#include "CentreFactory.h"
#include "Mosaic.h"
#include "PatternLine.h"
#include "WallManager.h"
#include "Wall.h"
#include "DiscardedLine.h"
#include "GameEngine.h"

typedef std::vector<std::vector<std::string>> SaveDataArgs;

#define PLAYER_FIELDS_LENGTH 8
#define FACTORY_FIELDS_LENGTH 7
#define GAME_FIELDS_LENGTH 7

class Loader {

  public:
    Loader();
    ~Loader();
    
    // Attempts to load a saved game from disk, returns false is failed
    bool load(std::string filename, GameEngine* gameEngine);

  private:
    bool error;

    // Reads data filds in from file and seperates them into key value pairs.
    SaveDataArgs parseData(std::ifstream* file, std::string line);

    // Parse type field on each data object.
    void parseTypes(SaveDataArgs args, GameEngine* gameEngine);
    
    // Parse data fields for Player objects.
    void parsePlayer(SaveDataArgs data, GameEngine* gameEngine);

    // Parse data fields for Factory objects.
    void parseFactory(SaveDataArgs data, GameEngine* gameEngine);

    // Parse data fields for Game objects.
    void parseGame(SaveDataArgs data, GameEngine* gameEngine);
    
    // Return true if the line starts with the comment symbol '//'.
    bool detectComments(std::string line);
    
    // Return true if the type field contains valid values.
    bool validateTypeField(SaveDataArgs args);

    // Return true if the file exists.
    bool validateFile(std::ifstream* file, std::string filename);

    // Return true if the data objcet has all its required fields.
    bool validateFields(SaveDataArgs args, std::string type, int length);
    
    // Flips error flag to false.
    void resetError();

};

#endif
