#include "../headers/AIPlayer.h"

AIPlayer::AIPlayer() {}

AIPlayer::~AIPlayer() {}

std::vector<std::string> AIPlayer::turn(int index, BaseEngine* gameEngine) {
  std::vector<std::string> move;

  move = canTurn(index, gameEngine) ? evaluateTurn(index, gameEngine)
                                    : evaluateDiscard(gameEngine);

  return move;
}

bool AIPlayer::canTurn(int index, BaseEngine* gameEngine) {
  bool result = false;

  Player* ai = gameEngine->getPlayer(index);

  for (int i = 0; i < ROWS; ++i) {
    bool rowNotFull = !ai->getMosaic()->getPattern()->isRowFull(i);

    if (rowNotFull) {
      PatternLine* pattern = ai->getMosaic()->getPattern();
      Colour colour = pattern->getColour(i);

      for (int j = 0; j < gameEngine->getFactoryLength(); ++j) {
        bool colourAvailable = gameEngine->getFactory(j)->contains(colour);

        if (colourAvailable || colour == EMPTY) {
          result = true;
        }
      }

      for (int j = 0; j < gameEngine->getCentreFactoryLength(); ++j) {
        bool colourAvailable =
            gameEngine->getCentreFactory(j)->contains(colour);

        if (colourAvailable || colour == EMPTY) {
          result = true;
        }
      }
    }
  }

  return result;
}

std::vector<std::string> AIPlayer::evaluateTurn(int index, BaseEngine* gameEngine) {
  std::vector<std::string> move;

  const int colourLength = 5;
  Colour colours[colourLength] = {BLACK, RED, YELLOW, LIGHT_BLUE, DARK_BLUE};

  std::string turn = "turn";
  std::string factory = "-1";
  std::string colour = "E";
  std::string pile = "-1";

  bool searching = true;

  Player* player = gameEngine->getPlayer(index);
  int offset = 0;
  int multiplier = 1;

  while (searching) {

    for (int i = 0; i < DIMENSIONS; ++i) {
      PatternRow* row = player->getMosaic()->getPattern()->getRow(i);
      WallManager* wall = player->getMosaic()->getWall();

      if (!row->isFull() && !wall->rowTrue(i)) {
        
        Colour rowColour = row->getColour() == EMPTY ?
          wall->nextAvailableColour(i) :
          row->getColour();

        int remaining = row->remaining();

        remaining -= remaining == 1 ? 0 : offset;

        // Normal factories
        for (int j = 0; j < gameEngine->getFactoryLength(); ++j) {
          Factory* factoryObj = gameEngine->getFactory(j);

          if (!factoryObj->isEmpty()) {
            bool colourMatch = factoryObj->contains(rowColour);
            bool remainingMatch = factoryObj->count(rowColour) == remaining;

            if (colourMatch && remainingMatch) {
              factory = std::to_string(j + 2);
              colour = std::string(1, rowColour);
              pile = std::to_string(i + 1);
              j = gameEngine->getFactoryLength();
              i = DIMENSIONS;
              searching = false;
            }
          }
        }

        // Centre factories
        if (searching) {
          for (int j = 0; j < gameEngine->getCentreFactoryLength(); ++j) {
            CentreFactory* factoryObj = gameEngine->getCentreFactory(j);

            if (!factoryObj->isEmpty()) {
              bool colourMatch = factoryObj->contains(rowColour);
              bool remainingMatch = factoryObj->count(rowColour) == remaining;

              if (colourMatch && remainingMatch) {
                factory = std::to_string(j);
                colour = std::string(1, rowColour);
                pile = std::to_string(i + 1);
                j = gameEngine->getFactoryLength();
                i = DIMENSIONS;
                searching = false;
              }
            }
          }
        } 
      }
    }

    offset += multiplier;

    if (offset > 5) {
      multiplier *= -1;
      offset = -1;
    }

    if (offset < -5) {
      for (int i = 0; i < DIMENSIONS; ++i) {
        PatternRow* row = player->getMosaic()->getPattern()->getRow(i);
        WallManager* wall = player->getMosaic()->getWall();

        if (row->isEmpty() && !wall->rowTrue(i)) {
          for (int j = 0; j < colourLength; ++j) {

            Colour rowColour = colours[j];

            if (!wall->isColourSet(rowColour, i)) {
              
              // Normal factories
              for (int k = 0; k < gameEngine->getFactoryLength(); ++k) {
                Factory* factoryObj = gameEngine->getFactory(k);
                
                if (!factoryObj->isEmpty() && factoryObj->contains(rowColour)) {
                  factory = std::to_string(k + 2);
                  colour = std::string(1, rowColour);
                  pile = std::to_string(i + 1);
                  j = colourLength;
                  i = DIMENSIONS;
                  k = gameEngine->getFactoryLength();
                  searching = false;
                }
              }

              // Centre factories
              if (searching) {
                for (int k = 0; k < gameEngine->getCentreFactoryLength(); k++) {
                CentreFactory* factoryObj = gameEngine->getCentreFactory(k);
                
                  if (!factoryObj->isEmpty() && factoryObj->contains(rowColour)) {
                    factory = std::to_string(k);
                    colour = std::string(1, rowColour);
                    pile = std::to_string(i + 1);
                    j = colourLength;
                    i = DIMENSIONS;
                    k = gameEngine->getFactoryLength();
                    searching = false;
                  }
                }
              }
            }   
          }
        }
      }
    }
  }

  move.push_back(turn);
  move.push_back(factory);
  move.push_back(colour);
  move.push_back(pile);

  std::cout << "TURN: " << turn + " " + factory + " " + colour + " " + pile << std::endl;

  return move;
}

int AIPlayer::evaluateCentreFactoryIndex(int index, BaseEngine* gameEngine) {
  CentreFactory** centreFactories = gameEngine->getCentreFactories();
  int size = gameEngine->getCentreFactoryLength();
  int result = -1;

  if (size > 1) {
    bool smallerFactory = centreFactories[0]->size() > centreFactories[1]->size();
    result = smallerFactory ? 1 : 0;
  } else {
    result = 0;
  }

  return result;
}

std::vector<std::string> AIPlayer::evaluateDiscard(BaseEngine* gameEngine) {
  std::vector<std::string> move;

  std::string discard = "discard";
  std::string factory = "-1";
  std::string colour = "E";

  const int colourLength = 5;
  Colour colours[colourLength] = {BLACK, RED, YELLOW, LIGHT_BLUE, DARK_BLUE};

  bool minimisingPointLossage = true;
  bool searching = true;
  int minimalColourCount = 1;

  while (minimisingPointLossage) {
    // Normal Factories
    for (int i = 0; i < gameEngine->getFactoryLength(); ++i) {
      if (!gameEngine->getFactory(i)->isEmpty()) {
        for (int j = 0; j < colourLength; ++j) {
          Colour next = colours[j];
          int colourCount = gameEngine->getFactory(i)->count(next);

          if (colourCount == minimalColourCount) {
            factory = std::to_string(i + 2);
            colour = std::string(1, next);
            searching = false;
            minimisingPointLossage = false;
            j = colourLength;
            i = gameEngine->getFactoryLength();
          }
        }
      }
    }

    // Centre Factories
    if (searching) {
      for (int i = 0; i < gameEngine->getCentreFactoryLength(); ++i) {
        if (!gameEngine->getCentreFactory(i)->isEmpty()) {
          for (int j = 0; j < colourLength; ++j) {
            Colour next = colours[j];
            int colourCount = gameEngine->getCentreFactory(i)->count(next);

            if (colourCount == minimalColourCount) {
              factory = std::to_string(i);
              colour = std::string(1, next);
              searching = false;
              minimisingPointLossage = false;
              j = colourLength;
              i = gameEngine->getCentreFactoryLength();
            }
          }
        }
      }
    }

    if (searching) {
      minimalColourCount++;
    }
  }

  move.push_back(discard);
  move.push_back(factory);
  move.push_back(colour);

  return move;
}
