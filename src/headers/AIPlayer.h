#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include <string>
#include <vector>

#include "BaseEngine.h"
#include "Player.h"
#include "Types.h"
#include "Colour.h"
#include "PatternLine.h"
#include "PatternRow.h"
#include "WallManager.h"
#include "Printer.h"

// Forward Declaration
class BaseEngine;

class AIPlayer {  

  public:
    AIPlayer();
    ~AIPlayer();

    // Decide if the ai can make a turn or discard, then evaluate the move
    std::vector<std::string> turn(int index, BaseEngine* gameEngine);

    int evaluateCentreFactoryIndex(int index, BaseEngine* gameEngine);
    
  private:

    Printer* printer;

    // Returns true if the ai can move a tile to the pattern line
    bool canTurn(int index, BaseEngine* gameEngine);

    // Evaluate move set for a turn command
    std::vector<std::string> evaluateTurn(int index, BaseEngine* gameEngine);

    // Evaluate discard
    std::vector<std::string> evaluateDiscard(BaseEngine* gameEngine);

    // Returns the index of a factory containing the required colours at a specified amount
    int queryFactoryColourAmount(int amount, Colour colour, BaseEngine* gameEngine);

    // Returns true if a colour exists in any factory
    bool queryFactoryColour(Colour colour, BaseEngine* gameEngine);

    // Returns the index of a factory containing the required colour
    int findMatchingColour(BaseEngine* gameEngine, PatternRow* row, int rowIndex,  WallManager* wall, Colour rowColour);

};


#endif
