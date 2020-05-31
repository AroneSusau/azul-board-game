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

    // Returns true if the ai can move a tile to the pattern line
    bool canTurn(int index, BaseEngine* gameEngine);

    // Evaluate move set for a turn command
    std::vector<std::string> evaluateTurn(int index, BaseEngine* gameEngine);

    // Evaluate discard
    std::vector<std::string> evaluateDiscard(BaseEngine* gameEngine);

};


#endif
