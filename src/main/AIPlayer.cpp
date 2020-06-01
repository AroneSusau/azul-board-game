#include "../headers/AIPlayer.h"

AIPlayer::AIPlayer(): printer(new Printer()) {}

AIPlayer::~AIPlayer() { delete printer; }

std::vector<std::string> AIPlayer::turn(int index, BaseEngine* gameEngine) {
  std::vector<std::string> move;

  move = canTurn(index, gameEngine) ? evaluateTurn(index, gameEngine)
                                    : evaluateDiscard(gameEngine);

  return move;
}

bool AIPlayer::canTurn(int index, BaseEngine* gameEngine) {
    
  const int colourLength = 5;
  Colour colours[colourLength] = {BLACK, RED, YELLOW, LIGHT_BLUE, DARK_BLUE};
  
  Player* ai = gameEngine->getPlayer(index);
  
  bool result = false;

  for (int i = 0; i < ROWS; ++i) {
    Colour colour = ai->getMosaic()->getPattern()->getColour(i);
    bool rowNotFull = !ai->getMosaic()->getPattern()->isRowFull(i);
    bool rowEmpty = colour == EMPTY;

    if (rowEmpty) {
      for (int j = 0; j < colourLength; ++j) {
        bool wallFree = !ai->getMosaic()->getWall()->isColourSet(colours[j], i);
        
        if (wallFree) {
          bool colourExists = queryFactoryColour(colours[j], gameEngine);
          
          if (colourExists) {
            result = true;
            i = ROWS;
            j = colourLength;
          }
        }
      } 
    } else if (rowNotFull) {
      if (queryFactoryColour(colour, gameEngine)) {
        result = true;
        i = ROWS;
      }
    }
  }

  return result;
}

std::vector<std::string> AIPlayer::evaluateTurn(int index, BaseEngine* gameEngine) {
  std::vector<std::string> move;

  std::string turn = "turn";
  std::string factory = "-1";
  std::string colour = "E";
  std::string pile = "-1";

  bool searching = true;
  const int colourLength = 5;
  int cycleLimit = 20;

  Player* player = gameEngine->getPlayer(index);

  while (searching) {

    for (int i = 0; i < DIMENSIONS; ++i) {
      PatternRow* row = player->getMosaic()->getPattern()->getRow(i);
      WallManager* wall = player->getMosaic()->getWall();
      Colour rowColour = row->getColour();

      // Case 1. Pattern line partially full
      if (!row->isFull() && !wall->rowTrue(i) && rowColour != EMPTY) {

        int result = -1;
        int offset = 0;
        int amount = 0;
        int quantity = row->getMax() - row->count();
        bool findingMatch = true;
        bool flipped = false;
        bool colourAvailable = queryFactoryColour(rowColour, gameEngine);

        if (colourAvailable) {
          while (findingMatch) {
            amount = quantity + offset;
            result = queryFactoryColourAmount(amount, rowColour, gameEngine);

            if (result != -1) {

              factory = std::to_string(result);
              colour = std::string(1, rowColour);
              pile = std::to_string(i + 1);
              i = DIMENSIONS;
              searching = false;
              findingMatch = false;

            } else if (result == -1 && amount < cycleLimit && !flipped) {
              offset++;
            } else if (result == -1 && amount >= cycleLimit && !flipped) {
              flipped = true;
              offset = -1;
            } else if (result == -1 && amount > 0 && flipped) {
              offset--;
            } else if (result == -1 && amount == 0 && flipped) {
              findingMatch = false;
            }
            amount = 0;
          }
        }

      // Case 2. Pattern line empty
      } else if (!row->isFull() && !wall->rowTrue(i) && rowColour == EMPTY) {
        for (int j = 0; j < colourLength; ++j) {
          int result = -1;
          int offset = 0;
          int quantity = row->getMax() - row->count();
          int amount = 0;
          bool findingMatch = true;
          bool flipped = false;
          rowColour = *wall->getColours()->get(i, j);
          bool colourSetOnWall = wall->isColourSet(rowColour, i);

          if (!colourSetOnWall) {
            bool colourAvailable = queryFactoryColour(rowColour, gameEngine);

            if (colourAvailable) {

              while (findingMatch) {
                amount = quantity + offset;
                result = queryFactoryColourAmount(amount, rowColour, gameEngine);

                if (result != -1) {

                  factory = std::to_string(result);
                  colour = std::string(1, rowColour);
                  pile = std::to_string(i + 1);
                  i = DIMENSIONS;
                  j = colourLength;
                  searching = false;
                  findingMatch = false;

                } else if (result == -1 && amount < cycleLimit && !flipped) {
                  offset++;
                } else if (result == -1 && amount >= cycleLimit && !flipped) {
                  flipped = true;
                  offset = -1;
                } else if (result == -1 && amount > 0 && flipped) {
                  offset--;
                } else if (result == -1 && amount == 0 && flipped) {
                  findingMatch = false;
                }
                amount = 0;
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
    
    int result = -1;

    for (int i = 0; i < colourLength; ++i) {
      result = queryFactoryColourAmount(minimalColourCount, colours[i], gameEngine);

      if (result != -1) {
        searching = false;
        minimisingPointLossage = false;
        factory = std::to_string(result);
        colour = std::string(1, colours[i]);
        i = colourLength;
      }
    }

    ++minimalColourCount;

    if (minimalColourCount > 10) {
      printer->error("Error->evaluateDiscard: No valid discard value found.");
    }
  }

  move.push_back(discard);
  move.push_back(factory);
  move.push_back(colour);

  return move;
}

int AIPlayer::queryFactoryColourAmount(int amount, Colour colour, BaseEngine* gameEngine) {

  int result = -1;

  // Normal Factories
  for (int i = 0; i < gameEngine->getFactoryLength(); ++i) {
    Factory* factory = gameEngine->getFactory(i);
    
    if (!factory->isEmpty()) {
      bool containsColour = factory->contains(colour);
      bool countMatch = factory->count(colour) == amount;

      if (containsColour && countMatch) {
        result = i + gameEngine->getCentreFactoryLength();
        i = gameEngine->getFactoryLength();
      }
    }
  }

  // Centre Factories
  for (int i = 0; i < gameEngine->getCentreFactoryLength(); ++i) {
    if (result == -1) {
      CentreFactory* factory = gameEngine->getCentreFactory(i);
      
      if (!factory->isEmpty()) {
        bool containsColour = factory->contains(colour);
        bool countMatch = factory->count(colour) == amount;

        if (containsColour && countMatch) {
          result = i;
          i = gameEngine->getCentreFactoryLength();
        }
      }
    } else {
      i = gameEngine->getCentreFactoryLength();
    }
  }

  return result;
}

bool AIPlayer::queryFactoryColour(Colour colour, BaseEngine* gameEngine) {
  bool result = false;

  // Normal Factories
  for (int i = 0; i < gameEngine->getFactoryLength(); ++i) {
    if (gameEngine->getFactory(i)->contains(colour)) {
      result = true;
      i = gameEngine->getFactoryLength();
    }
  }

  // Centre Factories
  for (int i = 0; i < gameEngine->getCentreFactoryLength(); ++i) {
    if (!result && gameEngine->getCentreFactory(i)->contains(colour)) {
      result = true;
      i = gameEngine->getCentreFactoryLength();
    }
  }

  return result;
}
